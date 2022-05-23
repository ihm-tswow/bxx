#include "tests.hpp"
#include "mesh.hpp"
#include "object.hpp"

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
    obj.set_location(1,2,3);
    BXX_ASSERT_EQUAL(obj.get_location(), bxx::vec3( 1,2,3 ));
}

BXX_TEST(rotate_object_euler) {
    bxx::object obj = create_test_object("test_object");
    obj.set_rotation_euler(1, 2, 3);
    BXX_ASSERT_EQUAL(obj.get_rotation_euler(), bxx::vec3(1, 2, 3));
}

BXX_TEST(rotate_object_quaternion) {
    bxx::object obj = create_test_object("test_object");
    obj.set_rotation_quaternion(-1, 0.1, 0.2, 0.3);
    BXX_ASSERT_EQUAL(obj.get_rotation_quaternion(), bxx::quaternion(-1, 0.1, 0.2, 0.3));
}

BXX_TEST(scale_object) {
    bxx::object obj = create_test_object("test_object");
    obj.set_scale(1, 2, 3);
    BXX_ASSERT_EQUAL(obj.get_scale(), bxx::vec3(1, 2, 3));
}
