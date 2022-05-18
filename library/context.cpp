#include "context.hpp"
#include "scene.hpp"
#include "exec.hpp"

void bxx::context::set_mode(bxx::editor_mode mode)
{
    switch (mode)
    {
    case bxx::editor_mode::EDIT:
        exec("bpy.ops.object.mode_set(mode='EDIT')");
        break;
    case bxx::editor_mode::EDIT_GPENCIL:
        exec("bpy.ops.object.mode_set(mode='EDIT_PENCIL')");
        break;
    case bxx::editor_mode::OBJECT:
        exec("bpy.ops.object.mode_set(mode='OBJECT')");
        break;
    case bxx::editor_mode::PAINT_GPENCIL:
        exec("bpy.ops.object.mode_set(mode='PAINT_PENCIL')");
        break;
    case bxx::editor_mode::PARTICLE_EDIT:
        exec("bpy.ops.object.mode_set(mode='PARTICLE_EDIT')");
        break;
    case bxx::editor_mode::POSE:
        exec("bpy.ops.object.mode_set(mode='POSE')");
        break;
    case bxx::editor_mode::SCULPT:
        exec("bpy.ops.object.mode_set(mode='SCULPT')");
        break;
    case bxx::editor_mode::SCULPT_GPENCIL:
        exec("bpy.ops.object.mode_set(mode='SCULPT_PENCIL')");
        break;
    case bxx::editor_mode::TEXTURE_PAINT:
        exec("bpy.ops.object.mode_set(mode='TEXTURE_PAINT')");
        break;
    case bxx::editor_mode::VERTEX_GPENCIL:
        exec("bpy.ops.object.mode_set(mode='VERTEX_GPENCIL')");
        break;
    case bxx::editor_mode::VERTEX_PAINT:
        exec("bpy.ops.object.mode_set(mode='VERTEX_PAINT')");
        break;
    case bxx::editor_mode::WEIGHT_GPENCIL:
        exec("bpy.ops.object.mode_set(mode='WEIGHT_GPENCIL')");
        break;
    case bxx::editor_mode::WEIGHT_PAINT:
        exec("bpy.ops.object.mode_set(mode='WEIGHT_PAINT')");
        break;
    }
}
