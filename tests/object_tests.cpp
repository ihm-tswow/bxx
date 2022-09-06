#include <bxx/blender_types/data.hpp>
#include <bxx/blender_types/mesh.hpp>
#include <bxx/blender_types/object.hpp>
#include <common/tests.hpp>

using namespace bxx;

static object create_test_object(std::string const& name)
{
    mesh mesh = get_data().meshes().create(name + "_mesh");
    return get_data().objects().create(name, mesh);
}

BXX_TEST(object_created_with_name) {
    BXX_ASSERT_EQUAL(create_test_object("test_object").get_name(), "test_object");
}

BXX_TEST(move_object) {
    object obj = create_test_object("test_object");
    obj.location().set(1,2,3);
    BXX_ASSERT_EQUAL(obj.location().get(), mathutils::vec3( 1,2,3 ));
}
BXX_TEST(rotate_object_euler) {
    object obj = create_test_object("test_object");
    obj.rotation_euler().set(1, 2, 3);
    BXX_ASSERT_EQUAL(obj.rotation_euler().get(), mathutils::vec3(1, 2, 3));
}

BXX_TEST(rotate_object_quaternion) {
    object obj = create_test_object("test_object");
    obj.rotation_quaternion().set(-1.0f, 0.1f, 0.2f, 0.3f);
    BXX_ASSERT_EQUAL(obj.rotation_quaternion().get(), mathutils::quaternion(-1.0f, 0.1f, 0.2f, 0.3f));
}

BXX_TEST(scale_object) {
    object obj = create_test_object("test_object");
    obj.scale().set(1, 2, 3);
    BXX_ASSERT_EQUAL(obj.scale().get(), mathutils::vec3(1, 2, 3));
}

BXX_TEST(object_children) {
    mesh m1 = get_data().meshes().create("m1");
    object o1 = get_data().objects().create("o1",m1);

    mesh m2 = get_data().meshes().create("m2");
    object o2 = get_data().objects().create("o2",m2);

    o2.set_parent(o1);
    BXX_ASSERT_EQUAL(o1.children().len(), 1);
}
