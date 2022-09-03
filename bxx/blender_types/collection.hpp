#pragma once

#include <bxx/objects/id.hpp>
#include <bxx/blender_types/blender_types.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_collection_types.h>
#include <makesdna/DNA_object_types.h>
#pragma warning(pop)

namespace bxx
{
    class object;
    class collection : public id<bl_collection>
    {
    public:
        std::string get_type_path() const final;
        void link_object(bxx::object const& obj);
    };
}