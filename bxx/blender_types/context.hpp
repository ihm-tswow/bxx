#pragma once

#include <bxx/blender_types/window_manager.hpp>
#include <bxx/objects/python_object.hpp>

#include <string>

namespace bxx
{
    class view_layer;
    class scene;
    class object;

    enum class editor_mode
    {
        OBJECT,
        EDIT,
        POSE,
        SCULPT,
        VERTEX_PAINT,
        WEIGHT_PAINT,
        TEXTURE_PAINT,
        PARTICLE_EDIT,
        EDIT_GPENCIL,
        SCULPT_GPENCIL,
        PAINT_GPENCIL,
        WEIGHT_GPENCIL,
        VERTEX_GPENCIL,
        UNKNOWN
    };

    class context : python_object
    {
    public:
        using python_object::python_object;
        PYFIELD_READ(bxx::window_manager, window_manager);
        void set_mode(editor_mode mode);
        editor_mode get_mode();
        view_layer get_view_layer();
        scene get_scene();
        void select_all();
        void deselect_all();
        void clear_active_object();
        void set_active_object(object const& object);
        object get_active_object();
        void link_object(object const& object);
        void update();
    };

    context get_context();
}