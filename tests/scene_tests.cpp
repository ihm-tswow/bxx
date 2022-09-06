#include <bxx/blender_types/data.hpp>
#include <bxx/blender_types/scene.hpp>
#include <bxx/blender_types/context.hpp>
#include <bxx/blender_types/view_layer.hpp>
#include <common/tests.hpp>

using namespace bxx;

BXX_TEST(create_scene_with_name) {
    BXX_ASSERT_EQUAL(get_data().scenes().create("test_scene").get_name(), "test_scene");
    BXX_ASSERT_EQUAL(get_data().scenes().create("test_scene_2").get_name(), "test_scene_2");
}

BXX_TEST(create_view_layer_with_name) {
    BXX_ASSERT_EQUAL(get_context().get_scene().view_layers().create("test_layer").get_name(), "test_layer");
    BXX_ASSERT_EQUAL(get_context().get_scene().view_layers().create("test_layer_2").get_name(), "test_layer_2");
}