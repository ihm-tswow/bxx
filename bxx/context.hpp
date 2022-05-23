#pragma once

namespace bxx
{
    class view_layer;
    class scene;

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
        VERTEX_GPENCIL
    };
    class context
    {
    public:
        static void set_mode(editor_mode mode);
        static view_layer get_view_layer();
        static scene get_scene();
    };
}