#pragma once

#include "id.hpp"
#include "mathutils.hpp"

#include <string>

typedef struct Image bl_image;

namespace bxx
{
    class image_buffer
    {
    public:
        image_buffer(float* buffer, uint64_t id, int width, int height);
        ~image_buffer();
        void set_index(int index, mathutils::rgba<255> const& rgba);
        void set_coord(int x, int y, mathutils::rgba<255> const& rgba);
        void set_index_channel(int index, int channel, float value);
        void set_coord_channel(int x, int y, int channel, float value);

        float get_index_channel(int index, int channel);
        float get_coord_channel(int x, int y, int channel);
        mathutils::rgba<255> get_index(int index);
        mathutils::rgba<255> get_coord(int x, int y);

        static bxx::image_buffer create(int width, int height);
        uint64_t get_id() const;
    private:
        int channel_to_index(int index, int channel);
        int coord_to_index(int x, int y, int channel);
        float* m_buffer;
        int m_width, m_height;
        uint64_t m_id;
    };

    class image : public id
    {
    public:
        static bxx::image create(std::string const& name, int width, int height);
        static bxx::image get(std::string const& name, int width, int height);

        image(bl_image * image, int width, int height);
        bl_image* get_raw();
        std::string get_name() const override;
        std::string get_full_name() const override;
        void apply_buffer(bxx::image_buffer const& buffer);
        bxx::image_buffer create_buffer();
        int get_width();
        int get_height();
    private:
        bl_image * m_raw;
        int m_width, m_height;
    };
}