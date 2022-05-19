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