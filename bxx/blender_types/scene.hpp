#pragma once

#include <bxx/objects/id.hpp>
#include <bxx/blender_types/blender_types.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_scene_types.h>
#pragma warning(pop)

namespace bxx
{
    class view_layer;

    class scene : public id<bl_scene>
    {
    public:
        using id<bl_scene>::id;
        view_layer add_view_layer(std::string const& name);
        view_layer get_view_layer(std::string const& name);
        static bxx::scene create(std::string const& name);
        static bxx::scene get(std::string const& name);
    private:
        bl_scene *m_raw;
    };
}