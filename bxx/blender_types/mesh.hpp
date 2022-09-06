#pragma once

#include <bxx/blender_types/blender_types.hpp>
#include <bxx/blender_types/iterables.hpp>
#include <bxx/objects/id.hpp>
#include <bxx/mathutils.hpp>

namespace bxx
{
    class deform_weight : public blender_struct<bl_deform_weight>
    {
    public:
        using blender_struct<bl_deform_weight>::blender_struct;
        float get_weight();
        void set_weight(float weight);
        int get_group_id();
        void set_group_id(int group);
    };

    namespace details
    {
        bl_deform_weight* get_deform_weight(bl_deform_vert* vert, size_t index);
        size_t deform_weight_len(bl_deform_vert* vert);
    }

    class deform_vert : public blender_ptr_iterable<bl_deform_vert, bl_deform_weight, deform_weight,details::deform_weight_len,details::get_deform_weight>
    {
    public:
        using blender_ptr_iterable<bl_deform_vert,bl_deform_weight,deform_weight,details::deform_weight_len, details::get_deform_weight>::blender_ptr_iterable;
        int get_flags();
        void set_flags(int flags);
    };

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

    namespace details
    {
        size_t vert_len(bl_mesh* m);
        bl_vert* get_vert(bl_mesh* m, size_t index);
        size_t deform_vert_len(bl_mesh* m);
        bl_deform_vert* get_deform_vert(bl_mesh* m, size_t index);
        size_t poly_len(bl_mesh* m);
        bl_poly* get_poly(bl_mesh* m, size_t index);
        size_t loop_len(bl_mesh* m);
        bl_loop* get_loop(bl_mesh* m, size_t index);
        bl_loop_uv* get_uv(bl_mesh* m, size_t index);
        bl_loop_col* get_color(bl_mesh* m, size_t index);
    }

    class mesh : public id<bl_mesh>
    {
    public:
        using id<bl_mesh>::id;

        void add_verts(int verts);
        void add_loops(int loops);
        void add_polygons(int polygons);

        void add_uv_channel(std::string const& name = "UV Channel");
        void add_color_channel(std::string const& name = "Color Channel");

        blender_ptr_iterable<bl_mesh, bl_vert, vert, details::vert_len, details::get_vert>
        verts();
        blender_ptr_iterable<bl_mesh, bl_deform_vert, deform_vert, details::deform_vert_len, details::get_deform_vert>
        deform_verts();
        blender_ptr_iterable<bl_mesh, bl_poly, poly, details::poly_len, details::get_poly>
        polys();
        blender_ptr_iterable<bl_mesh, bl_loop, loop, details::loop_len, details::get_loop>
        loops();
        blender_ptr_iterable<bl_mesh, bl_loop_uv, uv_wrapper, details::loop_len, details::get_uv>
        uvs();
        blender_ptr_iterable<bl_mesh, bl_loop_col, color, details::loop_len, details::get_color>
        colors();
    };

    class mesh_data : public blender_py_iterable<mesh>
    {
    public:
        using blender_py_iterable<mesh>::blender_py_iterable;
        mesh create(std::string const& name);
        mesh find(std::string const& name);
    };
}