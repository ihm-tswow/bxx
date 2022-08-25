import os
import bpy
import json
from cpython cimport array
from ...preferences import preferences
from ..common.auto_reload import AUTO_RELOAD_CONFIG_FILE, AUTO_RELOAD_LOCK_FILE
from ..third_party.lock import FileLock
from .util import get_addon_path, get_addon_name

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   - Exec Globals -
#
#   These are variables and functions that exec functions
#   can access.
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

registered_operators = {}

def register_operator(script,name,operator,show):
    global registered_operators
    if not script in registered_operators:
        registered_operators[script] = []
    registered_operators[script].append((operator,show))
    bpy.utils.register_class(operator)
    bpy.types.VIEW3D_MT_object.append(show)

cdef void unregister_script(char* script):
    script_str = script.decode('utf-8')
    if script_str in registered_operators:
        for (operator,show) in registered_operators[script_str]:
            bpy.types.VIEW3D_MT_object.remove(show)
            bpy.utils.unregister_class(operator)
        del registered_operators[script_str]

cdef extern void _fire_operator(char* script, char* operator, char* args)

def fire_operator(self,script,operator,arguments):
    script_b = script.encode('utf-8')
    operator_b = operator.encode('utf-8')
    obj = {}
    for arg in arguments:
        if not hasattr(self,arg):
            print(f"Error: missing argument {arg} in operator {operator}")
            continue
        obj[arg] = getattr(self,arg)
    obj_json_b = json.dumps(obj).encode('utf-8')
    _fire_operator(script_b,operator_b,obj_json_b)

registered_property_groups = []
def register_property_group(target, name, property_group,is_collection):
    registered_property_groups.append((target,name))
    if is_collection:
        setattr(target,name,bpy.props.CollectionProperty(type=property_group))
    else:
        setattr(target,name,bpy.props.PointerProperty(type=property_group))

cdef array.array cur_pixels
image_buffers = {}
cdef float* create_image_buffer(unsigned long long id, int width ,int height):
    global cur_pixels
    cur_pixels = array.array('f',[0])
    array.resize(cur_pixels,width*height*4)
    image_buffers[id] = cur_pixels
    return cur_pixels.data.as_floats

cdef void apply_image_buffer(unsigned long long buffer_id, char* image_name):
    image_name_str = image_name.decode('utf-8')
    image = bpy.data.images[image_name_str]
    buffer = image_buffers[buffer_id]
    image.pixels.foreach_set(buffer) # this looks slow, but it's very fast.

cdef void delete_image_buffer(unsigned long long id):
    del image_buffers[id]

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   - Cython Functions -
#
#   These are functions that we have to implement in cython.
#
#   They're currently in this file because it seems having them
#   anywhere else causes issues.
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

ctypedef unsigned long long cy_ptr_ct;
ctypedef void(*cy_exec_ct)(char*);
ctypedef cy_ptr_ct (*cy_eval_ptr_ct)(char*);
ctypedef int (*cy_eval_int_ct)(char*)
ctypedef float (*cy_eval_float_ct)(char*)
ctypedef char* (*cy_eval_string_ct)(char*)
ctypedef void (*cy_unregister_script_ct)(char*)

ctypedef float* (*cy_create_image_buffer_ct)(unsigned long long,int,int)
ctypedef void (*cy_apply_image_buffer_ct)(unsigned long long,char*)
ctypedef void (*cy_delete_image_buffer_ct)(unsigned long long)

def build_context():
    context = {}
    context['bpy'] = bpy
    context['register_operator'] = register_operator
    context['register_property_group'] = register_property_group
    context['preferences'] = preferences
    context['fire_operator'] = fire_operator
    context['get_addon_name'] = get_addon_name
    context['get_addon_path'] = get_addon_path
    return context

cdef void _exec(char* exec_bytes):
    try:
        exec(exec_bytes.decode('utf-8'), build_context())
    except Exception as e:
        raise Exception('Failed to execute python string:\n\n{0}'.format(exec_bytes.decode("utf-8"))) from e

def _eval(char* exec_bytes):
    try:
        context = build_context()
        exec(exec_bytes.decode('utf-8'), context)
        return context['out']
    except Exception as e:
        raise Exception('Failed to execute python string:\n\n{0}'.format(exec_bytes.decode("utf-8"))) from e

cdef cy_ptr_ct eval_ptr(char* exec_bytes):
    return _eval(exec_bytes)

cdef int eval_int(char* exec_bytes):
    return int(_eval(exec_bytes))

cdef float eval_float(char* exec_bytes):
    return float(_eval(exec_bytes))

last_str = None
cdef char* eval_string(char* exec_bytes):
    global last_str
    try:
        last_str = str(_eval(exec_bytes)).encode('utf-8')
    except:
        last_str = "".encode('utf-8')
    return last_str

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   - Entry -
#
#   These are the first functions we call
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

cdef extern int run_tests(char* incl, char* excl);
cdef extern void register_cxx();
cdef extern void unregister_cxx();
cdef extern void setup_cxx(
    char* path,
    cy_exec_ct cy_exec,
    cy_eval_ptr_ct cy_eval_ptr,
    cy_eval_int_ct cy_eval_int,
    cy_eval_float_ct cy_eval_float,
    cy_eval_string_ct cy_eval_string,
    cy_unregister_script_ct cy_unregister_script,
    cy_create_image_buffer_ct cy_create_image_buffer,
    cy_apply_image_buffer_ct cy_apply_image_buffer,
    cy_delete_image_buffer_ct cy_delete_image_buffer,
);
cdef extern void auto_reload_cxx();

def auto_reload_lockfile_path():
    return os.path.join(get_addon_path(), AUTO_RELOAD_LOCK_FILE)

if os.path.exists(auto_reload_lockfile_path()):
    try:
        os.remove(auto_reload_lockfile_path())
    except:
        print("Failed to remove lockfile, automatic reloading can become unstable")

setup_cxx(
    get_addon_path().encode('utf-8'),
    _exec,
    eval_ptr,
    eval_int,
    eval_float,
    eval_string,
    unregister_script,
    create_image_buffer,
    apply_image_buffer,
    delete_image_buffer
);

def auto_reload_delay():
    return preferences.get('auto_reload_interval',0.25)

def auto_reload():
    if os.path.exists(os.path.join(get_addon_path(), AUTO_RELOAD_CONFIG_FILE)):
        try:
            with FileLock(auto_reload_lockfile_path(),0.001):
                auto_reload_cxx()
        except:
            print("Libraries were busy, not loading")
    return auto_reload_delay()

def register():
    register_cxx()
    bpy.app.timers.register(auto_reload, first_interval=auto_reload_delay())

def unregister():
    global registered_operators
    for op_table in registered_operators:
        for (operator,show) in op_table:
            bpy.types.VIEW3D_MT_object.remove(show)
            bpy.utils.unregister_class(operator)

    for (target,name) in registered_property_groups:
        delattr(target,name)

    registered_operators = []
    unregister_cxx();

# called from test_runner.py, because blender does not accept cython methods
def _run_tests(incl,excl):
    ret = run_tests(incl,excl)
    bpy.app.timers.register(auto_reload, first_interval=auto_reload_delay())
    return ret

