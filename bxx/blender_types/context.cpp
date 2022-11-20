#include <bxx/blender_types/context.hpp>
#include <bxx/blender_types/view_layer.hpp>
#include <bxx/blender_types/scene.hpp>
#include <bxx/blender_types/object.hpp>
#include <bxx/blender_types/screen.hpp>
#include <bxx/enums.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx
{
    void context::set_mode(editor_mode mode)
    {
        exec("bpy.ops.object.mode_set(mode='{}')", std::string(enums::get_enum_name<editor_mode>(mode)));
    }

    view_layer context::get_view_layer()
    {
        // todo: is it _always_ the current scene containing the current view layer?
        return eval_pyobject("out = bpy.context.view_layer");
    }

    scene context::get_scene()
    {
        return eval_pyobject("out = bpy.context.scene");
    }

    editor_mode context::get_mode()
    {
        return enums::get_enum_value<editor_mode>(eval_string(
            "out = bpy.context.mode"
        ));
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

    void context::set_active_object(object const& object)
    {
        eval_pyobject("out = bpy.context.view_layer.objects").setattr("active", object);
    }

    object context::get_active_object()
    {
        return object(eval_pyobject("out = bpy.context.view_layer.objects.active"));
    }

    void context::link_object(object const& obj)
    {
        eval_pyobject("out = bpy.context.collection.objects").call("link", obj);
    }

    void context::update()
    {
        exec("bpy.context.view_layer.update()");
    }

    screen context::get_screen()
    {
        return getattr<screen>("screen");
    }

    context get_context()
    {
        return context(eval_pyobject("out = bpy.context"));
    }
}
