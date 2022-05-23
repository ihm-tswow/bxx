#include "mesh.hpp"
#include "exec.hpp"

#include "fmt/core.h"

#include "makesdna/DNA_mesh_types.h"
#include "makesdna/DNA_meshdata_types.h"

void bxx::vert::set(mathutils::vec3 vert)
{
    set(vert.x, vert.y, vert.z);
}

mathutils::vec3 bxx::vert::get()
{
    return { get_x(), get_y(), get_z() };
}

bxx::vert::vert(bl_vert* vert)
    : m_vert(vert)
{}
void bxx::vert::set_x(float value) 
{
    m_vert->co[0] = value; 
}
void bxx::vert::set_y(float value)
{
    m_vert->co[1] = value;
}
void bxx::vert::set_z(float value)
{
    m_vert->co[2] = value;
}
void bxx::vert::set(float x, float y, float z)
{
    set_x(x);
    set_y(y);
    set_z(z);
}

float bxx::vert::get_x()
{
    return m_vert->co[0];
}

float bxx::vert::get_y()
{
    return m_vert->co[1];
}
float bxx::vert::get_z()
{
    return m_vert->co[2];
}

bl_vert* bxx::vert::get_raw()
{
    return m_vert;
}

bxx::loop::loop(bl_loop* loop)
    : m_loop(loop)
{}
int bxx::loop::get_edge()
{
    return m_loop->e;
}

int bxx::loop::get_vert()
{
    return m_loop->v;
}

void bxx::loop::set_edge(int edge)
{
    m_loop->e = edge;
}

void bxx::loop::set_vert(int vert)
{
    m_loop->v = vert;
}

void bxx::loop::set(int vert, int edge)
{
    set_vert(vert);
    set_edge(edge);
}

bl_loop* bxx::loop::get_raw()
{
    return m_loop;
}

bxx::poly::poly(bl_poly* poly)
    : m_poly(poly)
{}

void bxx::poly::set_loop_start(int start)
{
    m_poly->loopstart = start;
}

void bxx::poly::set_loop_count(int count)
{
    m_poly->totloop = count;
}
void bxx::poly::set(int start, int count)
{
    set_loop_start(start);
    set_loop_count(count);
}
int bxx::poly::get_loop_start()
{
    return m_poly->loopstart;
}
int bxx::poly::get_loop_count()
{
    return m_poly->totloop;
}
short bxx::poly::get_material()
{
    return m_poly->mat_nr;
}
void bxx::poly::set_material(short material)
{
    m_poly->mat_nr = material;
}
bl_poly* bxx::poly::get_raw()
{
    return m_poly;
}

bxx::uv_wrapper::uv_wrapper(bl_loop_uv* uv)
    : m_uv(uv)
{}

void bxx::uv_wrapper::set_u(float u)
{
    m_uv->uv[0] = u;
}

void bxx::uv_wrapper::set_v(float v)
{
    m_uv->uv[1] = v;
}

void bxx::uv_wrapper::set(float u, float v)
{
    set_u(u);
    set_v(v);
}

float bxx::uv_wrapper::get_u()
{
    return m_uv->uv[0];
}

float bxx::uv_wrapper::get_v()
{
    return m_uv->uv[1];
}

bl_loop_uv* bxx::uv_wrapper::get_raw()
{
    return m_uv;
}

bxx::color::color(bl_loop_col* color)
    : m_col(color)
{}

void bxx::color::set_r(float r)
{
    m_col->r = r;
}

void bxx::color::set_g(float g)
{
    m_col->g = g;
}

void bxx::color::set_b(float b)
{
    m_col->b = b;
}

void bxx::color::set_a(float a)
{
    m_col->a = a;
}

void bxx::color::set(float r, float g, float b, float a)
{
    set_r(r);
    set_g(g);
    set_b(b);
    set_a(a);
}

float bxx::color::get_r()
{
    return m_col->r;
}

float bxx::color::get_g()
{
    return m_col->g;
}

float bxx::color::get_b()
{
    return m_col->b;
}

float bxx::color::get_a()
{
    return m_col->a;
}

bl_loop_col* bxx::color::get_raw()
{
    return m_col;
}

bxx::mesh::mesh(bl_mesh* raw)
    : m_raw(raw)
{}

bl_mesh* bxx::mesh::get_raw()
{
    return m_raw;
}

void bxx::mesh::add_verts(int verts)
{
    exec({
        fmt::format("{}.vertices.add({})", get_full_name(), verts)
    });
}

void bxx::mesh::add_loops(int loops)
{
    exec({
        fmt::format("{}.loops.add({})", get_full_name(), loops)
    });
}

void bxx::mesh::add_polygons(int polygons)
{
    exec({
        fmt::format("{}.polygons.add({})", get_full_name(), polygons)
    });
}

int bxx::mesh::get_num_verts()
{
    return m_raw->totvert;
}

int bxx::mesh::get_num_polys()
{
    return m_raw->totpoly;
}

int bxx::mesh::get_num_loops()
{
    return m_raw->totloop;
}

std::string bxx::mesh::get_name()
{
    return m_raw->id.name + 2;
}

std::string bxx::mesh::get_full_name()
{
    return fmt::format("bpy.data.meshes['{}']", get_name());
}

void bxx::mesh::add_uv_channel(std::string const& name)
{
    exec({
        fmt::format("bpy.data.meshes['{}'].uv_layers.new(name='{}')", get_name(), name)
    });
}

void bxx::mesh::add_color_channel(std::string const& name)
{
    exec({
        fmt::format("bpy.data.meshes['{}'].vertex_colors.new(name='{}')", get_name(), name)
    });
}

bxx::mesh bxx::mesh::create(std::string const& name)
{
    return bxx::mesh(eval_ptr<bl_mesh>({
        fmt::format("out = bpy.data.meshes.new(name='{}').as_pointer()", name)
    }));
}

bxx::vert bxx::mesh::vert(int index)
{
    return bxx::vert(m_raw->mvert+index);
}

bxx::poly bxx::mesh::poly(int poly)
{
    return bxx::poly(m_raw->mpoly+poly);
}

bxx::loop bxx::mesh::loop(int loop)
{
    return bxx::loop(m_raw->mloop+loop);
}

bxx::uv_wrapper bxx::mesh::uv(int loop)
{
    return bxx::uv_wrapper(m_raw->mloopuv+loop);
}

bxx::color bxx::mesh::color(int loop)
{
    return bxx::color(m_raw->mloopcol+loop);
}

mathutils::rgba<255> bxx::color::get()
{
    return { get_r(), get_g(), get_b(), get_a() };
}

void bxx::color::set(mathutils::rgba<255> color)
{
    set_r(color.r);
    set_g(color.g);
    set_b(color.b);
    set_a(color.a);
}
