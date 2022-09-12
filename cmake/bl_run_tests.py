import bpy
import os

addon_name = os.path.basename(os.path.dirname(os.path.dirname(os.path.dirname(__file__))))
bpy.ops.preferences.addon_enable(module=addon_name)

runner_name = addon_name.replace('.','_')
runner_name = runner_name.replace('-','_')
test_runner = getattr(bpy.ops.object,'unit_tests_' + runner_name)
test_runner(raise_on_failure=True)