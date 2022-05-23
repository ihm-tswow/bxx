#pragma once

#include "id.hpp"
#include "scene.hpp"

typedef struct ViewLayer bl_view_layer;

namespace bxx
{
    class object;
    class scene;
    class collection;

    class view_layer : public id
    {
    public:
        view_layer(bxx::scene const& scene, bl_view_layer* layer);
        std::string get_name() const override;
        std::string get_full_name() const override;
        void link_object(bxx::object const& obj);
        bl_view_layer* get_raw();
    private:
        bl_view_layer* m_raw;
        bxx::scene m_parent;
    };
}