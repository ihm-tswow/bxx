#include "context.hpp"
#include "exec.hpp"
#include "view_layer.hpp"
#include "scene.hpp"
#include "object.hpp"

#include "fmt/core.h"

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

bxx::view_layer bxx::context::get_view_layer()
{
    // todo: is it _always_ the current scene containing the current view layer?
    return bxx::view_layer(get_scene(), eval_ptr<bl_view_layer>({
        "out = bpy.context.view_layer.as_pointer()"
    }));
}

bxx::scene bxx::context::get_scene()
{
    return bxx::scene(eval_ptr<bl_scene>({
        "out = bpy.context.scene.as_pointer()"
    }));
}

bxx::editor_mode bxx::context::get_mode()
{
    std::string mode = eval_string({
        "out = bpy.context.mode"
    });

    if (mode == "OBJECT") return bxx::editor_mode::OBJECT;
    if (mode == "EDIT") return bxx::editor_mode::EDIT;
    if (mode == "POSE") return bxx::editor_mode::POSE;
    if (mode == "SCULPT") return bxx::editor_mode::SCULPT;
    if (mode == "VERTEX_PAINT") return bxx::editor_mode::VERTEX_PAINT;
    if (mode == "WEIGHT_PAINT") return bxx::editor_mode::WEIGHT_PAINT;
    if (mode == "TEXTURE_PAINT") return bxx::editor_mode::TEXTURE_PAINT;
    if (mode == "PARTICLE_EDIT") return bxx::editor_mode::PARTICLE_EDIT;
    if (mode == "EDIT_GPENCIL") return bxx::editor_mode::EDIT_GPENCIL;
    if (mode == "SCULPT_GPENCIL") return bxx::editor_mode::SCULPT_GPENCIL;
    if (mode == "PAINT_GPENCIL") return bxx::editor_mode::PAINT_GPENCIL;
    if (mode == "WEIGHT_GPENCIL") return bxx::editor_mode::WEIGHT_GPENCIL;
    if (mode == "VERTEX_GPENCIL") return bxx::editor_mode::VERTEX_GPENCIL;
    // todo: show warning here
    return bxx::editor_mode::UNKNOWN;
}

void bxx::context::select_all()
{
    exec("bpy.ops.object.select_all(action='SELECT')");
}

void bxx::context::deselect_all()
{
    exec("bpy.ops.object.select_all(action='DESELECT')");
}

void bxx::context::clear_active_object()
{
    exec({
        "bpy.context.view_layer.objects.active = None",
        "bpy.context.view_layer.update()"
    });
}

void bxx::context::set_active_object(bxx::object const& object)
{
    exec({
        fmt::format("bpy.context.view_layer.objects.active = {}", object.get_full_name()),
        "bpy.context.view_layer.update()"
    });
}

bxx::object bxx::context::get_active_object()
{
    return bxx::object(eval_ptr<bl_object>({
        "out = bpy.context.view_layer.objects.active.as_pointer()"
    }));
}

void bxx::context::link_object(bxx::object const& object)
{
    exec(fmt::format("bpy.context.collection.objects.link({})", object.get_full_name()));
}

void bxx::context::update()
{
    exec("bpy.context.view_layer.update()");
}

