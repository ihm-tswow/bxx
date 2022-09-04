#pragma once

#include <bxx/blender_types/blender_types.hpp>
#include <bxx/blender_types/scene.hpp>
#include <bxx/objects/id.hpp>

namespace bxx
{
    class object;
    class scene;
    class collection;

    class view_layer : public blender_py_struct<bl_view_layer>
    {
    public:
        view_layer(scene const& parent, bl_view_layer* layer);
        std::string get_name() const;
        std::string get_full_name() const;
        void link_object(object const& obj);
        void update();
        python_object get_pyobject();
    private:
        scene m_parent;
    };
}