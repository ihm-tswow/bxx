#include <bxx/app_handlers.hpp>
#include <bxx/objects/python_object.hpp>
#include <bxx/objects/python_tuple.hpp>
#include <bxx/blender_types/scene.hpp>
#include <bxx/script.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx::app::handlers
{
    void register_handler(std::string const& target, std::function<void(bxx::python_tuple)> func)
    {
        // todo: for some bizarre reason this stops working if i don't pass all varargs python-side,
        // but everything is still in the first tuple once it reaches C++
        size_t event_idx = lib_register_event([=](bxx::python_tuple tuple) {
            func(tuple.get<python_tuple>(0));
            return bxx::python_object();
        });
        exec({
            fmt::format("@bpy.app.handlers.persistent"),
            fmt::format("def event_{}_{}(*args):",get_script_index(),event_idx),
            fmt::format("    fire_event({},{}, args)",get_script_index(),event_idx),
            fmt::format("register_app_handler({}, bpy.app.handlers.{}, event_{}_{})", get_script_index(), target, get_script_index(), event_idx, target)
        });
    }

    void depsgraph_update_pre(std::function<void(scene)> callback)
    {
        register_handler("depsgraph_update_pre", [=](python_tuple tup) {
            callback(tup.get<python_object>(0));
        });
    }

    void depsgraph_update_post(std::function<void(scene, python_object)> callback)
    {
        register_handler("depsgraph_update_post", [=](python_tuple tup) {
            callback(tup.get<scene>(0), tup.get<python_object>(1));
        });
    }
    void frame_change_pre(std::function<void(scene)> callback)
    {
        register_handler("frame_change_pre", [=](python_tuple tup) {
            callback(tup.get<scene>(0));
        });
    }
    void frame_change_post(std::function<void(scene, python_object)> callback)
    {
        register_handler("frame_change_post", [=](python_tuple tup) {
            callback(tup.get<scene>(0), tup.get<python_object>(1));
        });
    }

    void load_factory_preferences_post(std::function<void()> callback)
    {
        register_handler("load_factory_preferences_post", [=](python_tuple tup) {
            callback();
        });
    }
    void load_factory_startup_post(std::function<void()> callback)
    {
        register_handler("load_factory_startup_post", [=](python_tuple tup) {
            callback();
        });
    }
    void load_post(std::function<void()> callback)
    {
        register_handler("load_post", [=](python_tuple tup) {
            callback();
        });
    }
    void load_pre(std::function<void()> callback)
    {
        register_handler("load_pre", [=](python_tuple tup) {
            callback();
        });
    }
    void redo_pre(std::function<void(scene)> callback)
    {
        register_handler("redo_pre", [=](python_tuple tup) {
            callback(tup.get<scene>(0));
        });
    }
    void redo_post(std::function<void(scene)> callback)
    {
        register_handler("redo_post", [=](python_tuple tup) {
            callback(tup.get<scene>(0));
        });
    }
    void render_cancel(std::function<void()> callback)
    {
        register_handler("render_cancel", [=](python_tuple tup) {
            callback();
        });
    }
    void render_complete(std::function<void(scene)> callback)
    {
        register_handler("render_complete", [=](python_tuple tup) {
            callback(tup.get<scene>(0));
        });
    }
    void render_init(std::function<void()> callback)
    {
        register_handler("render_init", [=](python_tuple tup) {
            callback();
        });
    }
    void render_post(std::function<void(scene)> callback)
    {
        register_handler("render_post", [=](python_tuple tup) {
            callback(tup.get<scene>(0));
        });
    }
    void render_pre(std::function<void(scene)> callback)
    {
        register_handler("render_pre", [=](python_tuple tup) {
            callback(tup.get<scene>(0));
        });
    }
    void render_stats(std::function<void()> callback)
    {
        register_handler("render_stats", [=](python_tuple tup) {
            callback();
        });
    }
    void render_write(std::function<void()> callback)
    {
        register_handler("render_write", [=](python_tuple tup) {
            callback();
        });
    }
    void save_post(std::function<void()> callback)
    {
        register_handler("save_post", [=](python_tuple tup) {
            callback();
        });
    }
    void save_pre(std::function<void()> callback)
    {
        register_handler("save_pre", [=](python_tuple tup) {
            callback();
        });
    }
    void undo_post(std::function<void(scene)> callback)
    {
        register_handler("undo_post", [=](python_tuple tup) {
            callback(tup.get<scene>(0));
        });
    }
    void undo_pre(std::function<void(scene)> callback)
    {
        register_handler("undo_pre", [=](python_tuple tup) {
            callback(tup.get<scene>(0));
        });
    }
    void version_update(std::function<void()> callback)
    {
        register_handler("version_update", [=](python_tuple tup) {
            callback();
        });
    }
}