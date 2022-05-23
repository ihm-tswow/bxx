#pragma once

#include "id.hpp"
#include "mathutils.hpp"

typedef struct Mesh bl_mesh;
typedef struct MVert bl_vert;
typedef struct MPoly bl_poly;
typedef struct MLoop bl_loop;
typedef struct MLoopUV bl_loop_uv;
typedef struct MLoopCol bl_loop_col;

namespace bxx
{
    class vert
    {
    public:
        vert(bl_vert* vert);
        void set_x(float value);
        void set_y(float value);
        void set_z(float value);
        void set(float x, float y, float z);
        void set(mathutils::vec3 vert);
        mathutils::vec3 get();
        float get_x();
        float get_y();
        float get_z();
        bl_vert* get_raw();
    private:
        bl_vert* m_vert;
    };

    class loop
    {
    public:
        loop(bl_loop* loop);
        int get_edge();
        int get_vert();
        void set_edge(int edge);
        void set_vert(int vert);
        void set(int vert, int edge = 0);
        bl_loop* get_raw();
    private:
        bl_loop* m_loop;
    };

    class poly
    {
    public:
        poly(bl_poly* poly);
        void set_loop_start(int start);
        void set_loop_count(int count);
        void set(int start, int count);
        int get_loop_start();
        int get_loop_count();
        short get_material();
        void set_material(short material);
        bl_poly* get_raw();
    private:
        bl_poly* m_poly;
    };

    class uv_wrapper
    {
    public:
        uv_wrapper(bl_loop_uv* uv);
        void set_u(float u);
        void set_v(float v);
        void set(float u, float v);
        float get_u();
        float get_v();
        bl_loop_uv* get_raw();
    private:
        bl_loop_uv* m_uv;
    };

    class color
    {
    public:
        color(bl_loop_col* color);
        void set_r(float r);
        void set_g(float g);
        void set_b(float b);
        void set_a(float a);
        void set(float r, float g, float b, float a = 255);
        mathutils::rgba<255> get();
        void set(mathutils::rgba<255> color);
        float get_r();
        float get_g();
        float get_b();
        float get_a();
        bl_loop_col* get_raw();
    private:
        bl_loop_col* m_col;
    };

    class mesh : public id
    {
    public:
        mesh(bl_mesh* raw);
        std::string get_name() const override;
        std::string get_full_name() const override;

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

        bl_mesh* get_raw();
        static bxx::mesh create(std::string const& name);
    private:
        bl_mesh* m_raw;
    };
}