import bpy
from .core_cy import _run_tests

class test_runner(bpy.types.Operator):
    bl_idname = 'tests.run'
    bl_label = "Run Unit Tests"
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

    def draw(self, context):
        self.layout.prop(self, 'incl', text = 'Include')
        self.layout.prop(self, 'excl', text = 'Exclude')

    def execute(self, context):
        incl_b = self.incl.encode('utf-8')
        excl_b = self.excl.encode('utf-8')
        _run_tests(incl_b,excl_b)
        return {'FINISHED'}

def show_test_runner(self, context):
    self.layout.operator(test_runner.bl_idname)