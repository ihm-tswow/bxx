#pragma once

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

    class context
    {
    public:
        void set_mode(editor_mode mode);
        bxx::editor_mode get_mode();
        view_layer get_view_layer();
        scene get_scene();
        void select_all();
        void deselect_all();
        void clear_active_object();
        void set_active_object(bxx::object const& object);
        bxx::object get_active_object();
        void link_object(bxx::object const& object);
        void update();
    };

    context get_context();
}