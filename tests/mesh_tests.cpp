#include "tests.hpp"
#include <bxx/blender_types/mesh.hpp>
#include <bxx/blender_types/context.hpp>
#include <common/exec.hpp>

BXX_TEST(mesh_created_with_name) {
    BXX_ASSERT_EQUAL(bxx::mesh::create("test_mesh").get_name(), "test_mesh");
}

BXX_TEST(mesh_starts_with_no_meshdata) {
    bxx::mesh mesh = bxx::mesh::create("test_mesh");
    BXX_ASSERT_EQUAL(mesh.get_num_verts(), 0);
    BXX_ASSERT_EQUAL(mesh.get_num_polys(), 0);
    BXX_ASSERT_EQUAL(mesh.get_num_loops(), 0);
}

static bxx::mesh create_basic_triangle()
{
    bxx::mesh mesh = bxx::mesh::create("test_mesh");
    mesh.add_verts(3);
    mesh.add_loops(3);
    mesh.add_polygons(1);
    mesh.loop(0).set(0);
    mesh.loop(1).set(1);
    mesh.loop(2).set(2);
    mesh.poly(0).set(0, 3);
    return mesh;
}

BXX_TEST(mesh_can_add_geometry) {
    bxx::mesh triangle = create_basic_triangle();
    BXX_ASSERT_EQUAL(triangle.get_num_verts(), 3);
    BXX_ASSERT_EQUAL(triangle.get_num_loops(), 3);
    BXX_ASSERT_EQUAL(triangle.get_num_polys(), 1);
}

BXX_TEST(mesh_can_add_channels) {
    bxx::mesh tri = create_basic_triangle();
    tri.add_uv_channel();
    tri.add_color_channel();
    tri.uv(0).set(0, 1);
    tri.uv(1).set(2, 3);
    tri.uv(2).set(4, 5);
    tri.color(0).set(0, 0, 0, 0);
    tri.color(1).set(0, 0, 0, 0);
    tri.color(2).set(0, 0, 0, 0);
}
