import bpy
import os

addon_name = os.path.basename(os.path.dirname(os.path.dirname(os.path.dirname(__file__))))
bpy.ops.preferences.addon_enable(module=addon_name)
test_runner = getattr(bpy.ops.unit_tests,addon_name)
test_runner(raise_on_failure=True)