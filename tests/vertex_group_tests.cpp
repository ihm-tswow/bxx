#include <bxx/blender_types/mesh.hpp>
#include <bxx/blender_types/object.hpp>
#include <bxx/blender_types/vertex_group.hpp>
#include <common/tests.hpp>

using namespace bxx;

BXX_TEST(vertex_group_create)
{
    mesh m = mesh::create("vertex_group_mesh");
    m.add_verts(3);
    object o = object::create("vertex_group_object", m);
    vertex_group g = o.add_vertex_group("vertex_group");
    g.add({ 0,1 }, 1, VertexAssignMode::ADD);
    BXX_ASSERT_EQUAL(g.weight(0), 1);
    BXX_ASSERT_EQUAL(g.weight(1), 1);
}

BXX_TEST(remove_vertex_group)
{
    mesh m = mesh::create("vertex_group_mesh");
    m.add_verts(1);
    object o = object::create("vertex_group_object", m);
    vertex_group g = o.add_vertex_group("vertex_group");
    g.add({ 0 }, 1, VertexAssignMode::ADD);
    g.remove({ 0 });
}