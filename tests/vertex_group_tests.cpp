#include <bxx/blender_types/data.hpp>
#include <bxx/blender_types/mesh.hpp>
#include <bxx/blender_types/object.hpp>
#include <bxx/blender_types/vertex_group.hpp>
#include <common/tests.hpp>

using namespace bxx;

BXX_TEST(vertex_group_create)
{
    mesh m = get_data().meshes().create("vertex_group_mesh");
    m.add_verts(3);
    object o = get_data().objects().create("vertex_group_object", m);
    vertex_group g = o.vertex_groups().add("vertex_group");
    g.add({ 0,1 }, 1, VertexAssignMode::ADD);
    BXX_ASSERT_EQUAL(g.weight(0), 1);
    BXX_ASSERT_EQUAL(g.weight(1), 1);
}

BXX_TEST(remove_vertex_group)
{
    mesh m = get_data().meshes().create("vertex_group_mesh");
    m.add_verts(1);
    object o = get_data().objects().create("vertex_group_object", m);
    vertex_group g = o.vertex_groups().add("vertex_group");
    g.add({ 0 }, 1, VertexAssignMode::ADD);
    g.remove({ 0 });
}

BXX_TEST(vertex_group_vertices)
{
    mesh m = get_data().meshes().create("vertex_group_mesh");
    m.add_verts(1);
    object o = get_data().objects().create("vertex_group_object", m);
    vertex_group g = o.vertex_groups().add("vertex_group");
    g.add({ 0 }, 0.5, VertexAssignMode::ADD);
    BXX_ASSERT_EQUAL(m.deform_verts().get(0).len(), 1);
    BXX_ASSERT_EQUAL(m.deform_verts().get(0).get(0).get_weight(), 0.5);
}

BXX_TEST(vertex_group_multiple_vertices)
{
    mesh m = get_data().meshes().create("vertex_group_mesh");
    m.add_verts(2);
    object o = get_data().objects().create("vertex_group_object", m);
    vertex_group g = o.vertex_groups().add("vertex_group");
    g.add({ 0 }, 0.5, VertexAssignMode::ADD);
    g.add({ 1 }, 0.75, VertexAssignMode::ADD);

    BXX_ASSERT_EQUAL(m.deform_verts().get(0).len(), 1);
    BXX_ASSERT_EQUAL(m.deform_verts().get(1).len(), 1);

    BXX_ASSERT_EQUAL(m.deform_verts().get(0).get(0).get_weight(), 0.5);
    BXX_ASSERT_EQUAL(m.deform_verts().get(1).get(0).get_weight(), 0.75);
}

BXX_TEST(vertex_group_multiple_groups)
{
    mesh m = get_data().meshes().create("vertex_group_mesh");
    m.add_verts(2);
    object o = get_data().objects().create("vertex_group_object", m);
    vertex_group g1 = o.vertex_groups().add("vertex_group_1");
    vertex_group g2 = o.vertex_groups().add("vertex_group_2");

    g1.add({ 0 }, 0.1f, VertexAssignMode::ADD);
    g2.add({ 0 }, 0.2f, VertexAssignMode::ADD);

    BXX_ASSERT_EQUAL(m.deform_verts().get(0).len(), 2);

    BXX_ASSERT_EQUAL(m.deform_verts().get(0).get(0).get_weight(), 0.1f);
    BXX_ASSERT_EQUAL(m.deform_verts().get(0).get(1).get_weight(), 0.2f);
}

BXX_TEST(vertex_group_deform_count)
{
    mesh m = get_data().meshes().create("vertex_group_mesh");
    m.add_verts(2);
    object o = get_data().objects().create("vertex_group_object", m);
    vertex_group g1 = o.vertex_groups().add("vertex_group_1");

    // only add the first vert
    g1.add({ 0 }, 0.1f, VertexAssignMode::ADD);

    BXX_ASSERT_EQUAL(m.deform_verts().get(0).len(), 1);
    BXX_ASSERT_EQUAL(m.deform_verts().get(1).len(), 0);

    m.add_verts(1);
    g1.add({ 2 }, 0.2f, VertexAssignMode::ADD);

    BXX_ASSERT_EQUAL(m.deform_verts().get(0).len(), 1);
    BXX_ASSERT_EQUAL(m.deform_verts().get(1).len(), 0);
    BXX_ASSERT_EQUAL(m.deform_verts().get(2).len(), 1);
}

BXX_TEST(vertex_group_num_deform_verts)
{
    mesh m = get_data().meshes().create("vertex_group_mesh");
    m.add_verts(2);
    BXX_ASSERT_EQUAL(m.deform_verts().len(), 0);

    object o = get_data().objects().create("vertex_group_object", m);
    vertex_group g1 = o.vertex_groups().add("vertex_group_1");
    BXX_ASSERT_EQUAL(m.deform_verts().len(), 0);

    g1.add({ 0 }, 1.0f, VertexAssignMode::ADD);
    BXX_ASSERT_EQUAL(m.deform_verts().len(), 2);

    g1.add({ 1 }, 1.0f, VertexAssignMode::ADD);
    BXX_ASSERT_EQUAL(m.deform_verts().len(), 2);
}

// changing raw weight affects python value
BXX_TEST(vertex_group_weight)
{
    mesh m = get_data().meshes().create("vertex_group_mesh");
    m.add_verts(1);

    object o = get_data().objects().create("vertex_group_object", m);
    vertex_group g = o.vertex_groups().add("vertex_group");
    g.add({ 0 }, 0.1f, VertexAssignMode::ADD);

    m.deform_verts().get(0).get(0).set_weight(0.2f);

    BXX_ASSERT_EQUAL(g.weight(0), 0.2f);
}