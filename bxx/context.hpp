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
        static void set_mode(editor_mode mode);
        static bxx::editor_mode get_mode();
        static view_layer get_view_layer();
        static scene get_scene();
        static void select_all();
        static void deselect_all();
        static void clear_active_object();
        static void set_active_object(bxx::object const& object);
        static bxx::object get_active_object();
        static void link_object(bxx::object const& object);
        static void update();
    };
}