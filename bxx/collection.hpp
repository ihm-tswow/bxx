#pragma once

#include "id.hpp"

typedef struct Collection bl_collection;

namespace bxx
{
    class object;
    class collection : public id
    {
    public:
        collection(bl_collection* raw);
        std::string get_name() const override;
        std::string get_full_name() const override;
        void link_object(bxx::object const& obj);
        bl_collection* get_raw();
    private:
        bl_collection* m_raw;
    };
}