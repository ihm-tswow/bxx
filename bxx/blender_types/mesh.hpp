#pragma once

#include <bxx/blender_types/blender_types.hpp>
#include <bxx/objects/id.hpp>
#include <bxx/mathutils.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_mesh_types.h>
#include <makesdna/DNA_meshdata_types.h>
#pragma warning(pop)

namespace bxx
{
    class vert : public blender_struct<bl_vert>
    {
    public:
        using blender_struct<bl_vert>::blender_struct;
        void set_x(float value);
        void set_y(float value);
        void set_z(float value);
        void set(float x, float y, float z);
        void set(mathutils::vec3 vert);
        mathutils::vec3 get();
        mathutils::vec3 get_normal();
        float get_x();
        float get_y();
        float get_z();
    };

    class loop : public blender_struct<bl_loop>
    {
    public:
        using blender_struct<bl_loop>::blender_struct;
        int get_edge();
        int get_vert();
        void set_edge(int edge);
        void set_vert(int vert);
        void set(int vert, int edge = 0);
    };

    class poly : public blender_struct<bl_poly>
    {
    public:
        using blender_struct<bl_poly>::blender_struct;
        void set_loop_start(int start);
        void set_loop_count(int count);
        void set(int start, int count);
        int get_loop_start();
        int get_loop_count();
        short get_material();
        void set_material(short material);
    };

    class uv_wrapper : public blender_struct<bl_loop_uv>
    {
    public:
        using blender_struct<bl_loop_uv>::blender_struct;
        void set_u(float u);
        void set_v(float v);
        void set(float u, float v);
        float get_u();
        float get_v();
    };

    class color : public blender_struct<bl_loop_col>
    {
    public:
        using blender_struct<bl_loop_col>::blender_struct;
        void set_r(float r);
        void set_g(float g);
        void set_b(float b);
        void set_a(float a);
        void set(float r, float g, float b, float a = 255);
        mathutils::rgba get();
        void set(mathutils::rgba color);
        float get_r();
        float get_g();
        float get_b();
        float get_a();
        bl_loop_col* get_raw();
    private:
        bl_loop_col* m_col;
    };

    class mesh : public id<bl_mesh>
    {
    public:
        using id<bl_mesh>::id;
        std::string get_type_path() const final;

        void add_verts(int verts);
        void add_loops(int loops);
        void add_polygons(int polygons);

        void add_uv_channel(std::string const& name = "UV Channel");
        void add_color_channel(std::string const& name = "Color Channel");

        int get_num_verts();
        int get_num_polys();
        int get_num_loops();

        bxx::vert vert(int index);
        bxx::poly poly(int poly);
        bxx::loop loop(int loop);
        bxx::uv_wrapper uv(int loop);
        bxx::color color(int loop);

        static bxx::mesh create(std::string const& name);
    };
}