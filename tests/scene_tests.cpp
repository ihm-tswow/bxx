#include "tests.hpp"
#include "scene.hpp"
#include "context.hpp"
#include "view_layer.hpp"

BXX_TEST(create_scene_with_name) {
    BXX_ASSERT_EQUAL(bxx::scene::create("test_scene").get_name(), "test_scene");
    BXX_ASSERT_EQUAL(bxx::scene::create("test_scene_2").get_name(), "test_scene_2");
}

BXX_TEST(create_view_layer_with_name) {
    BXX_ASSERT_EQUAL(bxx::context::get_scene().add_view_layer("test_layer").get_name(), "test_layer");
    BXX_ASSERT_EQUAL(bxx::context::get_scene().add_view_layer("test_layer_2").get_name(), "test_layer_2");
}