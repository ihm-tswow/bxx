#include <bxx/blender_types/context.hpp>
#include <bxx/blender_types/view_layer.hpp>
#include <bxx/blender_types/scene.hpp>
#include <bxx/blender_types/object.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>
#include <magic_enum.hpp>

namespace bxx
{
    void context::set_mode(bxx::editor_mode mode)
    {
        exec("bpy.ops.object.mode_set(mode='{}')", std::string(magic_enum::enum_name<bxx::editor_mode>(mode)));
    }

    view_layer context::get_view_layer()
    {
        // todo: is it _always_ the current scene containing the current view layer?
        return bxx::view_layer(get_scene(), eval_ptr<bl_view_layer>("out = bpy.context.view_layer.as_pointer()"));
    }

    scene context::get_scene()
    {
        return bxx::scene(eval_ptr<bl_scene>("out = bpy.context.scene.as_pointer()"));
    }

    editor_mode context::get_mode()
    {
        return magic_enum::enum_cast<editor_mode>(eval_string(
            "out = bpy.context.mode"
        )).value();
    }

    void context::select_all()
    {
        exec("bpy.ops.object.select_all(action='SELECT')");
    }

    void context::deselect_all()
    {
        exec("bpy.ops.object.select_all(action='DESELECT')");
    }

    void context::clear_active_object()
    {
        exec({
            "bpy.context.view_layer.objects.active = None",
            "bpy.context.view_layer.update()"
            });
    }

    void context::set_active_object(bxx::object const& object)
    {
        exec({
            fmt::format("bpy.context.view_layer.objects.active = {}", object.get_name_full()),
            "bpy.context.view_layer.update()"
        });
    }

    object context::get_active_object()
    {
        return object(eval_ptr<bl_object>("out = bpy.context.view_layer.objects.active.as_pointer()"));
    }

    void context::link_object(object const& obj)
    {
        exec("bpy.context.collection.objects.link({})", obj.get_name_full());
    }

    void context::update()
    {
        exec("bpy.context.view_layer.update()");
    }

    context get_context()
    {
        return context();
    }
}

