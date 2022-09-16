import os
import bpy
import json
from cpython cimport array
from ..common.auto_reload import AUTO_RELOAD_CONFIG_FILE, AUTO_RELOAD_LOCK_FILE
from ..third_party.lock import FileLock
from .util import get_addon_path, get_addon_name
from cpython.ref cimport PyObject

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   - Defines -
#
#   Type definitions and forwards
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Library API
ctypedef unsigned long long cy_ptr_ct;
ctypedef int(*cy_exec_ct)(int,char*);
ctypedef PyObject* (*cy_eval_ct)(int,char*);
ctypedef float* (*cy_create_float_buffer_ct)(unsigned long long,int)

# Core API
cdef extern PyObject* core_fire_event(size_t script, size_t event, PyObject* state)
cdef extern int run_tests(char* incl, char* excl);
cdef extern void register_cxx();
cdef extern void unregister_cxx();
cdef extern void setup_cxx(
    char* path,
    cy_exec_ct cy_exec,
    cy_eval_ct cy_eval,
    cy_create_float_buffer_ct cy_create_float_buffer
);
cdef extern void auto_reload_cxx();

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   - Cython State -
#
#   These are the variables we own
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# python objects we need to unregister with the scripts
registered_operators = {}
registered_property_groups = {}
registered_app_handlers = {}
registered_classes = {}

class python_functions: pass
registered_python_functions = python_functions() # maps function names -> function callback
registered_cxx_functions = {}                                   # maps function names -> (script_id,event_id)

# image buffers
cdef array.array cur_pixels # image buffer create temp
image_buffers = {}

# eval
last_str = None # string eval temp
last_obj = None

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   - Exec API -
#
#   These are variables and functions that exec functions
#   can access.
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

def register_class(script,cls):
    global registered_classes
    if not script in registered_classes:
        registered_classes[script] = []
    registered_classes[script].append(cls)
    bpy.utils.register_class(cls)

def register_operator(script,operator,show):
    global registered_operators
    if not script in registered_operators:
        registered_operators[script] = []
    registered_operators[script].append((operator,show))
    bpy.utils.register_class(operator)
    bpy.types.VIEW3D_MT_object.append(show)

def register_cxx_function(name,script,event):
    global registered_cxx_functions
    if name in registered_cxx_functions:
        raise Exception("c++ function " + name + " is already registered")
    registered_cxx_functions[name] = (script,event)

def unregister_script(script):
    if script in registered_operators:
        for (op,show) in registered_operators[script]:
             bpy.types.VIEW3D_MT_object.remove(show)
             bpy.utils.unregister_class(op)
        registered_operators[script] = []

    if script in registered_property_groups:
        for (target,name) in registered_property_groups[script]:
            delattr(target,name)
        registered_property_groups[script] = []

    if script in registered_app_handlers:
        for (target,func) in registered_app_handlers[script]:
            target.remove(func)
        registered_app_handlers[script] = []

    if script in registered_classes:
        for cls in registered_classes[script]:
            bpy.utils.unregister_class(cls)
        registered_classes[script] = []

    removed_names = []
    for func_name,(func_script,func_event) in registered_cxx_functions.items():
        if script == func_script:
            removed_names.append(func_name)

    for name in removed_names:
        del registered_cxx_functions[name]


def fire_event(script,event,*args):
    obj = core_fire_event(script,event,<PyObject*>args)
    if not obj:
        raise Exception("Failed to execute C++ event")
    return <object> obj

def register_property_group(script,target, name, property_group,is_collection):
    if not script in registered_property_groups:
        registered_property_groups[script] = []
    registered_property_groups[script].append((target,name))
    if is_collection:
        setattr(target,name,bpy.props.CollectionProperty(type=property_group))
    else:
        setattr(target,name,bpy.props.PointerProperty(type=property_group))

def register_app_handler(script, target, func):
    target.append(func)
    if not script in registered_app_handlers:
        registered_app_handlers[script] = []
    registered_app_handlers[script].append((target,func))

cdef float* create_float_buffer(unsigned long long id, int size):
    global cur_pixels
    cur_pixels = array.array('f',[0])
    array.resize(cur_pixels,size)
    image_buffers[id] = cur_pixels
    return cur_pixels.data.as_floats

def apply_image_buffer(buffer_id, image_name):
    image = bpy.data.images[image_name]
    buffer = image_buffers[buffer_id]
    image.pixels.foreach_set(buffer) # this looks slow, but it's very fast.

def delete_float_buffer(id):
    del image_buffers[id]

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   - Library API -
#
#   Cython functions we expose to libraries
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

def build_context(script):
    context = {}
    context['bpy'] = bpy
    context['register_cxx_function'] = register_cxx_function
    context['registered_python_functions'] = registered_python_functions
    context['unregister_script'] = unregister_script
    context['apply_image_buffer'] = apply_image_buffer
    context['delete_float_buffer'] = delete_float_buffer
    context['register_operator'] = register_operator
    context['register_class'] = register_class
    context['fire_event'] = fire_event
    context['register_property_group'] = register_property_group
    context['register_app_handler'] = register_app_handler
    context['get_addon_name'] = get_addon_name
    context['get_addon_path'] = get_addon_path
    if script in registered_classes:
        for cls in registered_classes[script]:
            context[cls.__name__] = cls
    return context

cdef int cy_exec(int script, char* exec_bytes):
    try:
        exec(exec_bytes.decode('utf-8'), build_context(script))
        return 0
    except Exception as e:
        print(e)
        return -1

cdef PyObject* cy_eval(int script, char* exec_bytes):
    global last_obj
    try:
        context = build_context(script)
        exec(exec_bytes.decode('utf-8'), context)
        last_obj = context['out']
        return <PyObject*> last_obj
    except Exception as e:
        print(e)
        return NULL

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   - Core API -
#
#   Cython functions we expose to the rest of the core
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

def auto_reload_lockfile_path():
    return os.path.join(get_addon_path(), AUTO_RELOAD_LOCK_FILE)

def auto_reload():
    if os.path.exists(os.path.join(get_addon_path(), AUTO_RELOAD_CONFIG_FILE)):
        try:
            with FileLock(auto_reload_lockfile_path(),0.001):
                auto_reload_cxx()
        except:
            print("Libraries were busy, not loading")
    return 0.25

def register():
    register_cxx()
    bpy.app.timers.register(auto_reload, first_interval=0.25, persistent=True)

def unregister():
    unregister_cxx();

# called from test_runner.py, because blender does not accept cython methods
def cy_run_tests(incl,excl):
    ret = run_tests(incl,excl)
    return ret

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   - Startup -
#
#   Code we fire as soon as we load
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Remove lockfile
if os.path.exists(auto_reload_lockfile_path()):
    try:
        os.remove(auto_reload_lockfile_path())
    except:
        print("Failed to remove lockfile, automatic reloading can become unstable")

# Give our pointers to the core
setup_cxx(
    get_addon_path().encode('utf-8'),
    cy_exec,
    cy_eval,
    create_float_buffer
);
