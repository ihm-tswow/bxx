#include <bxx/blender_types/mesh.hpp>
#include <bxx/blender_types/object.hpp>
#include <common/tests.hpp>

static bxx::object create_test_object(std::string const& name)
{
    bxx::mesh mesh = bxx::mesh::create(name + "_mesh");
    return bxx::object::create(name, mesh);
}

BXX_TEST(object_created_with_name) {
    BXX_ASSERT_EQUAL(create_test_object("test_object").get_name(), "test_object");
}

BXX_TEST(move_object) {
    bxx::object obj = create_test_object("test_object");
    obj.location().set(1,2,3);
    BXX_ASSERT_EQUAL(obj.location().get(), mathutils::vec3( 1,2,3 ));
}
BXX_TEST(rotate_object_euler) {
    bxx::object obj = create_test_object("test_object");
    obj.rotation_euler().set(1, 2, 3);
    BXX_ASSERT_EQUAL(obj.rotation_euler().get(), mathutils::vec3(1, 2, 3));
}

BXX_TEST(rotate_object_quaternion) {
    bxx::object obj = create_test_object("test_object");
    obj.rotation_quaternion().set(-1.0f, 0.1f, 0.2f, 0.3f);
    BXX_ASSERT_EQUAL(obj.rotation_quaternion().get(), mathutils::quaternion(-1.0f, 0.1f, 0.2f, 0.3f));
}

BXX_TEST(scale_object) {
    bxx::object obj = create_test_object("test_object");
    obj.scale().set(1, 2, 3);
    BXX_ASSERT_EQUAL(obj.scale().get(), mathutils::vec3(1, 2, 3));
}
