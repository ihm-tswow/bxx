#include <bxx/blender_types/image.hpp>
#include <common/exec.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_image_types.h>
#pragma warning(pop)

#include <fmt/core.h>

namespace bxx
{
    void image::apply_buffer(image_buffer const& buffer)
    {
        get_pointers()->cy_apply_image_buffer(
            buffer.get_id(),
            const_cast<char*>(get_name().c_str())
        );
    }

    std::uint32_t image::get_width()
    {
        return getattr("size").get_item<int>(0);
    }

    std::uint32_t image::get_height()
    {
        return getattr("size").get_item<int>(1);
    }

    image_buffer image::create_buffer()
    {
        return image_buffer::create(get_width(), get_height());
    }

    uint64_t image_buffer::get_id() const
    {
        return m_id;
    }

    image_buffer image_buffer::create(int width, int height)
    {
        static uint64_t index = 0;
        uint64_t cur_index = index++;
        float* buffer = get_pointers()->cy_create_float_buffer(cur_index, width * height * 4);
        return image_buffer(buffer, cur_index, width, height);
    }

    image_buffer::image_buffer(float* buffer, uint64_t id, int width, int height)
        : m_buffer(buffer)
        , m_id(id)
        , m_width(width)
        , m_height(height)
    {}

    image_buffer::~image_buffer()
    {
        get_pointers()->cy_delete_image_buffer(m_id);
    }

    void image_buffer::set_index_channel(int index, int channel, float value)
    {
        *(m_buffer + channel_to_index(index, channel)) = value;
    }

    void image_buffer::set_coord_channel(int x, int y, int channel, float value)
    {
        *(m_buffer + coord_to_index(x, y, channel)) = value;
    }

    int image_buffer::channel_to_index(int index, int channel)
    {
        return index * 4 + channel;
    }

    int image_buffer::coord_to_index(int x, int y, int channel)
    {
        return channel_to_index(y + x * m_height, channel);
    }

    void image_buffer::set_index(int index, mathutils::rgba const& rgba)
    {
        set_index_channel(index, 0, rgba.r);
        set_index_channel(index, 1, rgba.g);
        set_index_channel(index, 2, rgba.b);
        set_index_channel(index, 3, rgba.a);
    }

    void image_buffer::set_coord(int x, int y, mathutils::rgba const& rgba)
    {
        set_coord_channel(x, y, 0, rgba.r);
        set_coord_channel(x, y, 1, rgba.g);
        set_coord_channel(x, y, 2, rgba.b);
        set_coord_channel(x, y, 3, rgba.a);
    }

    float image_buffer::get_index_channel(int index, int channel)
    {
        return *(m_buffer + channel_to_index(index, channel));
    }

    float image_buffer::get_coord_channel(int x, int y, int channel)
    {
        return *(m_buffer + coord_to_index(x, y, channel));
    }

    mathutils::rgba image_buffer::get_index(int index)
    {
        return {
            get_index_channel(index, 0),
            get_index_channel(index, 1),
            get_index_channel(index, 2),
            get_index_channel(index, 3),
        };
    }

    mathutils::rgba image_buffer::get_coord(int x, int y)
    {
        return {
            get_coord_channel(x,y, 0),
            get_coord_channel(x,y, 1),
            get_coord_channel(x,y, 2),
            get_coord_channel(x,y, 3),
        };
    }
}
