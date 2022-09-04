#include <bxx/blender_types/mesh.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx
{
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

    std::string mesh::get_type_path() const
    {
        return "bpy.data.meshes";
    }

    void mesh::add_verts(int verts)
    {
        exec("{}.vertices.add({})", get_name_full(), verts);
    }

    void mesh::add_loops(int loops)
    {
        exec("{}.loops.add({})", get_name_full(), loops);
    }

    void mesh::add_polygons(int polygons)
    {
        exec("{}.polygons.add({})", get_name_full(), polygons);
    }

    int mesh::get_num_verts()
    {
        return get_raw_struct()->totvert;
    }

    int mesh::get_num_polys()
    {
        return get_raw_struct()->totpoly;
    }

    int mesh::get_num_loops()
    {
        return get_raw_struct()->totloop;
    }

    void mesh::add_uv_channel(std::string const& name)
    {
        exec("bpy.data.meshes['{}'].uv_layers.new(name='{}')", get_name(), name);
    }

    void mesh::add_color_channel(std::string const& name)
    {
        exec("bpy.data.meshes['{}'].vertex_colors.new(name='{}')", get_name(), name);
    }

    mesh mesh::create(std::string const& name)
    {
        return mesh(eval_ptr<bl_mesh>(
            "out = bpy.data.meshes.new(name='{}').as_pointer()", name
        ));
    }

    vert mesh::vert(int index)
    {
        return get_raw_struct()->mvert + index;
    }

    poly mesh::poly(int poly)
    {
        return get_raw_struct()->mpoly + poly;
    }

    loop mesh::loop(int loop)
    {
        return get_raw_struct()->mloop + loop;
    }

    uv_wrapper mesh::uv(int loop)
    {
        return get_raw_struct()->mloopuv + loop;
    }

    color mesh::color(int loop)
    {
        return get_raw_struct()->mloopcol + loop;
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
}

