#include <bxx/blender_types/collection.hpp>
#include <bxx/blender_types/mesh.hpp>
#include <bxx/blender_types/object.hpp>
#include <common/tests.hpp>

using namespace bxx;

BXX_TEST(collection_object) {
    mesh m = mesh::create("m");
    object o = object::create("o", m);
    collection c = collection::create("c");
    c.objects().add(o);
    BXX_ASSERT_EQUAL(c.objects().len(), 1);
}
BXX_TEST(collection_child) {
    collection c1 = collection::create("c1");
    collection c2 = collection::create("c2");
    c1.children().add(c2);
    BXX_ASSERT_EQUAL(c1.children().len(), 1);
}
