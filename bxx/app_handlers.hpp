#pragma once

#include <functional>

namespace bxx
{
    class python_object;
    class python_tuple;
    class scene;
    namespace app::handlers
    {
        void depsgraph_update_pre(std::function<void(scene)> callback);
        void depsgraph_update_post(std::function<void(scene, python_object)> callback);
        void frame_change_pre(std::function<void(scene)> callback);
        void frame_change_post(std::function<void(scene, python_object)> callback);
        void load_factory_preferences_post(std::function<void()> callback);
        void load_factory_startup_post(std::function<void()> callback);
        void load_post(std::function<void()> callback);
        void load_pre(std::function<void()> callback);
        void redo_pre(std::function<void(scene)> callback);
        void redo_post(std::function<void(scene)> callback);
        // todo: unknown arguments
        void render_cancel(std::function<void()> callback);
        void render_complete(std::function<void(scene)> callback);
        // todo: unknown arguments
        void render_init(std::function<void()> callback);
        void render_post(std::function<void(scene)> callback);
        void render_pre(std::function<void(scene)> callback);
        // todo: unknown arguments
        void render_stats(std::function<void()> callback);
        // todo: unknown arguments
        void render_write(std::function<void()> callback);
        void save_post(std::function<void()> callback);
        void save_pre(std::function<void()> callback);
        void undo_post(std::function<void(scene)> callback);
        void undo_pre(std::function<void(scene)> callback);
        void version_update(std::function<void()> callback);
        void register_handler(std::string const& target, std::function<void(python_tuple)> callback);
    }
}