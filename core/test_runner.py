import bpy
from .core_cy import _run_tests
from .util import get_addon_name

class test_runner(bpy.types.Operator):
    bl_idname = 'unit_tests.{0}'.format(get_addon_name())
    bl_label = 'Unit Tests: {0}'.format(get_addon_name())
    bl_options = {'REGISTER','UNDO'}

    incl: bpy.props.StringProperty(
        name = "Include",
        description = "Test ids to include",
        default= "",
    )

    excl: bpy.props.StringProperty(
        name = "Exclude",
        description = "Test ids to exclude",
        default = "",
    )

    raise_on_failure: bpy.props.BoolProperty(
        name = "Produce Output",
        description = "Whether to write the exit code to a file",
        default = True
    )

    def draw(self, context):
        self.layout.prop(self, 'incl', text = 'Include')
        self.layout.prop(self, 'excl', text = 'Exclude')

    def execute(self, context):
        incl_b = self.incl.encode('utf-8')
        excl_b = self.excl.encode('utf-8')
        res = _run_tests(incl_b,excl_b)
        if(self.raise_on_failure):
            raise Exception("Tests failed")
        return {'FINISHED'}

def show_test_runner(self, context):
    self.layout.operator(test_runner.bl_idname)