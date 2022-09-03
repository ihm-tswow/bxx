#include "tests.hpp"
#include <bxx/blender_types/action.hpp>

BXX_TEST(create_action_with_name) {
    BXX_ASSERT_EQUAL(bxx::action::create("test_action").get_name(), "test_action");
}

BXX_TEST(create_fcurve) {
    bxx::action action = bxx::action::create("test_action");
    bxx::fcurve fcurve1 = action.add_fcurve("test_group", "test_name", 0);
    bxx::fcurve fcurve2 = action.add_fcurve("test_group", "test_name", 1);

    BXX_ASSERT_EQUAL(fcurve1.get_data_path(),"test_name");
    BXX_ASSERT_EQUAL(fcurve1.get_index(),0);

    BXX_ASSERT_EQUAL(fcurve2.get_data_path(),"test_name");
    BXX_ASSERT_EQUAL(fcurve2.get_index(),1);
}

BXX_TEST(create_keyframes) {
    bxx::fcurve curve = bxx::action::create("test_action")
        .add_fcurve("test_group", "test_name", 0);
    curve.add_points(10);

    BXX_ASSERT_EQUAL(curve.num_points(), 10);

    curve.get_point(0).set(1, 2);
    curve.get_point(1).set(3, 4);

    BXX_ASSERT_EQUAL(curve.get_point(0).get_time(), 1);
    BXX_ASSERT_EQUAL(curve.get_point(0).get_value(), 2);
    BXX_ASSERT_EQUAL(curve.get_point(1).get_time(), 3);
    BXX_ASSERT_EQUAL(curve.get_point(1).get_value(), 4);
}
