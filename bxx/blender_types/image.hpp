#pragma once

#include <bxx/blender_types/blender_types.hpp>
#include <bxx/objects/id.hpp>
#include <bxx/mathutils.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_image_types.h>
#pragma warning(pop)

#include <string>

namespace bxx
{
    class image_buffer
    {
    public:
        image_buffer(float* buffer, uint64_t id, int width, int height);
        ~image_buffer();
        void set_index(int index, mathutils::rgba const& rgba);
        void set_coord(int x, int y, mathutils::rgba const& rgba);
        void set_index_channel(int index, int channel, float value);
        void set_coord_channel(int x, int y, int channel, float value);

        float get_index_channel(int index, int channel);
        float get_coord_channel(int x, int y, int channel);
        mathutils::rgba get_index(int index);
        mathutils::rgba get_coord(int x, int y);

        static image_buffer create(int width, int height);
        uint64_t get_id() const;
    private:
        int channel_to_index(int index, int channel);
        int coord_to_index(int x, int y, int channel);
        float* m_buffer;
        int m_width, m_height;
        uint64_t m_id;
    };

    class image : public id<bl_image>
    {
    public:
        static image create(std::string const& name, int width, int height);
        static image get(std::string const& name, int width, int height);
        image(python_object const& obj, int width, int height);
        void apply_buffer(image_buffer const& buffer);
        image_buffer create_buffer();
        int get_width();
        int get_height();
    private:
        bl_image * m_raw;
        int m_width, m_height;
    };
}