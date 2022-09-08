#include <bxx/blender_types/data.hpp>
#include <bxx/blender_types/image.hpp>
#include <common/tests.hpp>

using namespace bxx;

BXX_TEST(create_image)
{
    image image = get_data().images().create("test_image", 1, 2);
    BXX_ASSERT_EQUAL(image.get_name(), "test_image");
    BXX_ASSERT_EQUAL(image.get_width(), 1);
    BXX_ASSERT_EQUAL(image.get_height(), 2);
}

BXX_TEST(create_float_buffer)
{
    image_buffer buffer = image_buffer::create(10, 10);
}

BXX_TEST(image_buffer_by_index)
{
    image_buffer buffer = image_buffer::create(2, 2);
    buffer.set_index(0, { 1,2,3,4 });
    BXX_ASSERT_EQUAL(buffer.get_index(0), mathutils::rgba(1, 2, 3, 4));

    buffer.set_index(1, { 5,6,7,8 });
    BXX_ASSERT_EQUAL(buffer.get_index(1), mathutils::rgba(5, 6, 7, 8));
}

BXX_TEST(image_buffer_by_coord)
{
    image_buffer buffer = image_buffer::create(10,10);
    buffer.set_coord(0, 0, { 1,2,3,4 });
    BXX_ASSERT_EQUAL(buffer.get_coord(0, 0), mathutils::rgba( 1,2,3,4 ));

    buffer.set_coord(1, 1, { 5,6,7,8 });
    BXX_ASSERT_EQUAL(buffer.get_coord(1, 1), mathutils::rgba( 5,6,7,8 ));

    buffer.set_coord(1, 0, { 9,10,11,12 });
    BXX_ASSERT_EQUAL(buffer.get_coord(1, 0), mathutils::rgba( 9,10,11,12 ));

    buffer.set_coord(0, 1, { 13,14,15,16 });
    BXX_ASSERT_EQUAL(buffer.get_coord(0, 1), mathutils::rgba( 13,14,15,16 ));
}

BXX_TEST(apply_image_buffer)
{
    image image = get_data().images().create("test_image", 10, 10);
    image_buffer buffer = image.create_buffer();
    image.apply_buffer(buffer);
}
