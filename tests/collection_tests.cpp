#include <bxx/blender_types/data.hpp>
#include <bxx/blender_types/collection.hpp>
#include <bxx/blender_types/mesh.hpp>
#include <bxx/blender_types/object.hpp>
#include <common/tests.hpp>

using namespace bxx;

BXX_TEST(collection_object) {
    mesh m = get_data().meshes().create("m");
    object o = get_data().objects().create("o", m);
    collection c = get_data().collections().create("c");
    c.objects().add(o);
    BXX_ASSERT_EQUAL(c.objects().len(), 1);
}
BXX_TEST(collection_child) {
    collection c1 = get_data().collections().create("c1");
    collection c2 = get_data().collections().create("c2");
    c1.children().add(c2);
    BXX_ASSERT_EQUAL(c1.children().len(), 1);
}
