#include "mesh.hpp"
#include "exec.hpp"

#include "fmt/core.h"

#include "makesdna/DNA_mesh_types.h"

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

