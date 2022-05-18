#pragma once

namespace bxx
{
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
        static void set_mode(editor_mode mode);
    };
}