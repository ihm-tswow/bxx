#include <common/tests.hpp>
#include <bxx/blender_types/action.hpp>
#include <bxx/blender_types/data.hpp>

using namespace bxx;

BXX_TEST(create_action_with_name) {
    BXX_ASSERT_EQUAL(get_data().actions().create("test_action").get_name(), "test_action");
}

BXX_TEST(create_fcurve) {
    action action = get_data().actions().create("test_action");
    fcurve fcurve1 = action.fcurves().add("test_group", "test_name", 0);
    fcurve fcurve2 = action.fcurves().add("test_group", "test_name", 1);

    BXX_ASSERT_EQUAL(fcurve1.get_data_path(),"test_name");
    BXX_ASSERT_EQUAL(fcurve1.get_index(),0);

    BXX_ASSERT_EQUAL(fcurve2.get_data_path(),"test_name");
    BXX_ASSERT_EQUAL(fcurve2.get_index(),1);
}

BXX_TEST(create_keyframes) {
    fcurve curve = get_data().actions().create("test_action")
        .fcurves().add("test_group", "test_name", 0);
    curve.add_points(10);

    BXX_ASSERT_EQUAL(curve.points().len(), 10);

    curve.points().get(0).set(1, 2);
    curve.points().get(1).set(3, 4);

    BXX_ASSERT_EQUAL(curve.points().get(0).get_time(), 1);
    BXX_ASSERT_EQUAL(curve.points().get(0).get_value(), 2);
    BXX_ASSERT_EQUAL(curve.points().get(1).get_time(), 3);
    BXX_ASSERT_EQUAL(curve.points().get(1).get_value(), 4);
}
