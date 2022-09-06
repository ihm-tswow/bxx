#include <bxx/blender_types/mesh.hpp>
#include <common/exec.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_mesh_types.h>
#include <makesdna/DNA_meshdata_types.h>
#pragma warning(pop)

#include <fmt/core.h>

namespace bxx
{
    float deform_weight::get_weight()
    {
        return get_raw_struct()->weight;
    }

    void deform_weight::set_weight(float weight)
    {
        get_raw_struct()->weight = weight;
    }

    int deform_weight::get_group_id()
    {
        return get_raw_struct()->def_nr;
    }

    void deform_weight::set_group_id(int group)
    {
        get_raw_struct()->def_nr = group;
    }

    int deform_vert::get_flags()
    {
        return this->get_raw_struct()->flag;
    }

    void deform_vert::set_flags(int flags)
    {
        this->get_raw_struct()->flag = flags;
    }

    void vert::set(mathutils::vec3 vert)
    {
        set(vert.x, vert.y, vert.z);
    }

    mathutils::vec3 vert::get()
    {
        return { get_x(), get_y(), get_z() };
    }

    void vert::set_x(float value)
    {
        get_raw_struct()->co[0] = value;
    }
    void vert::set_y(float value)
    {
        get_raw_struct()->co[1] = value;
    }
    void vert::set_z(float value)
    {
        get_raw_struct()->co[2] = value;
    }
    void vert::set(float x, float y, float z)
    {
        set_x(x);
        set_y(y);
        set_z(z);
    }

    float vert::get_x()
    {
        return get_raw_struct()->co[0];
    }

    float vert::get_y()
    {
        return get_raw_struct()->co[1];
    }
    float vert::get_z()
    {
        return get_raw_struct()->co[2];
    }

    mathutils::vec3 vert::get_normal()
    {
        return {
            float(get_raw_struct()->no[0]),
            float(get_raw_struct()->no[1]),
            float(get_raw_struct()->no[2])
        };
    }

    int loop::get_edge()
    {
        return get_raw_struct()->e;
    }

    int loop::get_vert()
    {
        return get_raw_struct()->v;
    }

    void loop::set_edge(int edge)
    {
        get_raw_struct()->e = edge;
    }

    void loop::set_vert(int vert)
    {
        get_raw_struct()->v = vert;
    }

    void loop::set(int vert, int edge)
    {
        set_vert(vert);
        set_edge(edge);
    }

    void poly::set_loop_start(int start)
    {
        get_raw_struct()->loopstart = start;
    }

    void poly::set_loop_count(int count)
    {
        get_raw_struct()->totloop = count;
    }
    void poly::set(int start, int count)
    {
        set_loop_start(start);
        set_loop_count(count);
    }
    int poly::get_loop_start()
    {
        return get_raw_struct()->loopstart;
    }
    int poly::get_loop_count()
    {
        return get_raw_struct()->totloop;
    }
    short poly::get_material()
    {
        return get_raw_struct()->mat_nr;
    }
    void poly::set_material(short material)
    {
        get_raw_struct()->mat_nr = material;
    }

    void uv_wrapper::set_u(float u)
    {
        get_raw_struct()->uv[0] = u;
    }

    void uv_wrapper::set_v(float v)
    {
        get_raw_struct()->uv[1] = v;
    }

    void uv_wrapper::set(float u, float v)
    {
        set_u(u);
        set_v(v);
    }

    float uv_wrapper::get_u()
    {
        return get_raw_struct()->uv[0];
    }

    float uv_wrapper::get_v()
    {
        return get_raw_struct()->uv[1];
    }

    void color::set_r(float r)
    {
        get_raw_struct()->r = std::uint8_t(r);
    }

    void color::set_g(float g)
    {
        get_raw_struct()->g = std::uint8_t(g);
    }

    void color::set_b(float b)
    {
        get_raw_struct()->b = std::uint8_t(b);
    }

    void color::set_a(float a)
    {
        get_raw_struct()->a = std::uint8_t(a);
    }

    void color::set(float r, float g, float b, float a)
    {
        set_r(r);
        set_g(g);
        set_b(b);
        set_a(a);
    }

    float color::get_r()
    {
        return get_raw_struct()->r;
    }

    float color::get_g()
    {
        return get_raw_struct()->g;
    }

    float color::get_b()
    {
        return get_raw_struct()->b;
    }

    float color::get_a()
    {
        return get_raw_struct()->a;
    }

    bl_loop_col* color::get_raw()
    {
        return get_raw_struct();
    }

    void mesh::add_verts(int verts)
    {
        getattr("vertices").call("add", verts);
    }

    void mesh::add_loops(int loops)
    {
        getattr("loops").call("add", loops);
    }

    void mesh::add_polygons(int polygons)
    {
        getattr("polygons").call("add", polygons);
    }

    void mesh::add_uv_channel(std::string const& name)
    {
        exec("bpy.data.meshes['{}'].uv_layers.new(name='{}')", get_name(), name);
    }

    void mesh::add_color_channel(std::string const& name)
    {
        exec("bpy.data.meshes['{}'].vertex_colors.new(name='{}')", get_name(), name);
    }

    mathutils::rgba color::get()
    {
        return { get_r(), get_g(), get_b(), get_a() };
    }

    void color::set(mathutils::rgba color)
    {
        set_r(color.r);
        set_g(color.g);
        set_b(color.b);
        set_a(color.a);
    }

    blender_ptr_iterable<bl_mesh, bl_vert, vert, details::vert_len, details::get_vert>
        mesh::verts() { return get_raw_struct(); }
    blender_ptr_iterable<bl_mesh, bl_deform_vert, deform_vert, details::deform_vert_len, details::get_deform_vert>
        mesh::deform_verts() { return get_raw_struct(); }
    blender_ptr_iterable<bl_mesh, bl_poly, poly, details::poly_len, details::get_poly>
        mesh::polys() { return get_raw_struct(); }
    blender_ptr_iterable<bl_mesh, bl_loop, loop, details::loop_len, details::get_loop>
        mesh::loops() { return get_raw_struct(); }
    blender_ptr_iterable<bl_mesh, bl_loop_uv, uv_wrapper, details::loop_len, details::get_uv>
        mesh::uvs() { return get_raw_struct(); }
    blender_ptr_iterable<bl_mesh, bl_loop_col, color, details::loop_len, details::get_color>
        mesh::colors() { return get_raw_struct(); }

    namespace details
    {
        bl_deform_weight* get_deform_weight(bl_deform_vert* vert, size_t index)
        {
            return vert->dw + index;
        }

        size_t deform_weight_len(bl_deform_vert* vert)
        {
            return vert->totweight;
        }

        size_t vert_len(bl_mesh* m)
        {
            return m->totvert;
        }

        bl_vert* get_vert(bl_mesh* m,size_t index)
        {
            return m->mvert + index;
        }

        size_t deform_vert_len(bl_mesh* m)
        {
            return m->dvert ? m->totvert : 0;
        }

        bl_deform_vert* get_deform_vert(bl_mesh* m,size_t index)
        {
            return m->dvert + index;
        }

        size_t poly_len(bl_mesh* m)
        {
            return m->totpoly;
        }

        bl_poly* get_poly(bl_mesh* m,size_t index)
        {
            return m->mpoly + index;
        }

        size_t loop_len(bl_mesh* m)
        {
            return m->totloop;
        }

        bl_loop* get_loop(bl_mesh* m,size_t index)
        {
            return m->mloop + index;
        }

        bl_loop_uv* get_uv(bl_mesh* m,size_t index)
        {
            return m->mloopuv + index;
        }

        bl_loop_col* get_color(bl_mesh* m,size_t index)
        {
            return m->mloopcol + index;
        }
    }
}
