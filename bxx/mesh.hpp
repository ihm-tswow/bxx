#pragma once

#include "id.hpp"

typedef struct Mesh bl_mesh;

namespace bxx
{
    class mesh : public id
    {
    public:
        mesh(bl_mesh* raw);
        std::string get_name() override;
        std::string get_full_name() override;

        void add_verts(int verts);
        void add_loops(int loops);
        void add_polygons(int polygons);

        void add_uv_channel(std::string const& name = "UV Channel");
        void add_color_channel(std::string const& name = "Color Channel");

        int get_num_verts();
        int get_num_polys();
        int get_num_loops();

        bl_mesh* get_raw();
    private:
        bl_mesh* m_raw;
    };
}