#include <bxx/blender_types/data.hpp>
#include <bxx/blender_types/mesh.hpp>
#include <bxx/blender_types/context.hpp>
#include <common/exec.hpp>
#include <common/tests.hpp>

using namespace bxx;

BXX_TEST(mesh_created_with_name) {
    BXX_ASSERT_EQUAL(get_data().meshes().create("test_mesh").get_name(), "test_mesh");
}

BXX_TEST(mesh_starts_with_no_meshdata) {
    mesh mesh = get_data().meshes().create("test_mesh");
    BXX_ASSERT_EQUAL(mesh.verts().len(), 0);
    BXX_ASSERT_EQUAL(mesh.polys().len(), 0);
    BXX_ASSERT_EQUAL(mesh.loops().len(), 0);
}

static mesh create_basic_triangle()
{
    mesh mesh = get_data().meshes().create("test_mesh");
    mesh.add_verts(3);
    mesh.add_loops(3);
    mesh.add_polygons(1);
    mesh.loops().get(0).set(0);
    mesh.loops().get(1).set(1);
    mesh.loops().get(2).set(2);
    mesh.polys().get(0).set(0, 3);
    return mesh;
}

BXX_TEST(mesh_can_add_geometry) {
    mesh triangle = create_basic_triangle();
    BXX_ASSERT_EQUAL(triangle.verts().len(), 3);
    BXX_ASSERT_EQUAL(triangle.loops().len(), 3);
    BXX_ASSERT_EQUAL(triangle.polys().len(), 1);
}

BXX_TEST(mesh_can_add_channels) {
    mesh tri = create_basic_triangle();
    tri.add_uv_channel();
    tri.add_color_channel();
    tri.uvs().get(0).set(0, 1);
    tri.uvs().get(1).set(2, 3);
    tri.uvs().get(2).set(4, 5);
    tri.colors().get(0).set(0, 0, 0, 0);
    tri.colors().get(1).set(0, 0, 0, 0);
    tri.colors().get(2).set(0, 0, 0, 0);
}
