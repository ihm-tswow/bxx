import bpy
from . import core_cy
from . import test_runner

def register():
    core_cy.register()
    bpy.utils.register_class(test_runner.test_runner)
    bpy.types.VIEW3D_MT_object.append(test_runner.show_test_runner)

def unregister():
    bpy.types.VIEW3D_MT_object.remove(test_runner.show_test_runner)
    bpy.utils.unregister_class(test_runner.test_runner)
    core_cy.unregister()

def register_python_function(name,func):
    setattr(core_cy.registered_python_functions,name,func)

def call_cxx_function(name,*args):
    if not name in core_cy.registered_cxx_functions:
        raise Exception("No cxx function called " + name)
    (script_id,event_id) = core_cy.registered_cxx_functions[name]
    return core_cy.fire_event(script_id,event_id,*args)