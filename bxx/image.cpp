#include "image.hpp"
#include "exec.hpp"

#include "makesdna/DNA_image_types.h"

#include "fmt/core.h"

bxx::image bxx::image::create(std::string const& name, int width, int height)
{
    return bxx::image(eval_ptr<bl_image>({
        fmt::format("out = bpy.data.images.new(name='{}', width={}, height={}).as_pointer()", name, width, height)
    }), width, height);
}

bxx::image bxx::image::get(std::string const& name, int width, int height)
{
    return bxx::image(eval_ptr <bl_image> ({
        fmt::format("out = bpy.data.images['{}']", name)
    }), width, height);
}

bxx::image::image(bl_image* image, int width ,int height)
    : m_raw(image)
    , m_width(width)
    , m_height(height)
{}

bl_image* bxx::image::get_raw()
{
    return m_raw;
}

std::string bxx::image::get_name() const
{
    return m_raw->id.name + 2;
}

std::string bxx::image::get_full_name() const
{
    return fmt::format("bpy.data.images['{}']", get_name());
}

void bxx::image::apply_buffer(bxx::image_buffer const& buffer)
{
    get_pointers()->cy_apply_image_buffer(
        buffer.get_id(),
        const_cast<char*>(get_name().c_str())
    );
}

bxx::image_buffer bxx::image::create_buffer()
{
    return bxx::image_buffer::create(m_width, m_height);
}


int bxx::image::get_width()
{
    return m_width;
}

int bxx::image::get_height()
{
    return m_height;
}

uint64_t bxx::image_buffer::get_id() const
{
    return m_id;
}

bxx::image_buffer bxx::image_buffer::create(int width, int height)
{
    static uint64_t index = 0;
    uint64_t cur_index = index++;
    float* buffer = get_pointers()->cy_create_image_buffer(cur_index, width, height);
    return bxx::image_buffer(buffer, cur_index, width, height);
}

bxx::image_buffer::image_buffer(float* buffer, uint64_t id, int width, int height)
    : m_buffer(buffer)
    , m_id(id)
    , m_width(width)
    , m_height(height)
{}

bxx::image_buffer::~image_buffer()
{
    get_pointers()->cy_delete_image_buffer(m_id);
}

void bxx::image_buffer::set_index_channel(int index, int channel, float value)
{
    *(m_buffer + channel_to_index(index, channel)) = value;
}

void bxx::image_buffer::set_coord_channel(int x, int y, int channel, float value)
{
    *(m_buffer + coord_to_index(x, y, channel)) = value;
}

int bxx::image_buffer::channel_to_index(int index, int channel)
{
    return index * 4 + channel;
}

int bxx::image_buffer::coord_to_index(int x, int y, int channel)
{
    return channel_to_index(y + x * m_height, channel);
}

void bxx::image_buffer::set_index(int index, mathutils::rgba<255> const& rgba)
{
    set_index_channel(index, 0, rgba.r);
    set_index_channel(index, 1, rgba.g);
    set_index_channel(index, 2, rgba.b);
    set_index_channel(index, 3, rgba.a);
}

void bxx::image_buffer::set_coord(int x, int y, mathutils::rgba<255> const& rgba)
{
    set_coord_channel(x, y, 0, rgba.r);
    set_coord_channel(x, y, 1, rgba.g);
    set_coord_channel(x, y, 2, rgba.b);
    set_coord_channel(x, y, 3, rgba.a);
}

float bxx::image_buffer::get_index_channel(int index, int channel)
{
    return *(m_buffer + channel_to_index(index, channel));
}

float bxx::image_buffer::get_coord_channel(int x, int y, int channel)
{
    return *(m_buffer + coord_to_index(x, y, channel));
}

mathutils::rgba<255> bxx::image_buffer::get_index(int index)
{
    return {
        get_index_channel(index, 0),
        get_index_channel(index, 1),
        get_index_channel(index, 2),
        get_index_channel(index, 3),
    };
}

mathutils::rgba<255> bxx::image_buffer::get_coord(int x, int y)
{
    return {
        get_coord_channel(x,y, 0),
        get_coord_channel(x,y, 1),
        get_coord_channel(x,y, 2),
        get_coord_channel(x,y, 3),
    };
}
