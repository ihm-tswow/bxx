#pragma once

#include <bxx/objects/id.hpp>
#include <bxx/blender_types/blender_types.hpp>
#include <bxx/blender_types/iterables.hpp>
#include <bxx/blender_types/object.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_collection_types.h>
#include <makesdna/DNA_object_types.h>
#pragma warning(pop)

namespace bxx
{
    class collection_objects;
    class collection_children;

    class collection : public id<bl_collection>
    {
    public:
        using id<bl_collection>::id;
        void link_object(object const& obj);
        collection_objects objects();
        collection_children children();
        static collection create(std::string const& name);
    };

    class collection_objects : public blender_py_iterable<object>
    {
    public:
        using blender_py_iterable<object>::blender_py_iterable;
        void add(object obj);
    };

    class collection_children : public blender_py_iterable<collection>
    {
    public:
        using blender_py_iterable<collection>::blender_py_iterable;
        void add(collection child);
    };
}