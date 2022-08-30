#pragma once

#include "id.hpp"

typedef struct Scene bl_scene;

namespace bxx
{
    class view_layer;

    class scene : public id
    {
    public:
        scene(bl_scene* raw);
        scene(python_object obj);
        void set_name(std::string const& name);
        std::string get_name() const override;
        std::string get_full_name() const override;
        view_layer add_view_layer(std::string const& name);
        view_layer get_view_layer(std::string const& name);
        static bxx::scene create(std::string const& name);
        static bxx::scene get(std::string const& name);
    private:
        bl_scene *m_raw;
    };
}