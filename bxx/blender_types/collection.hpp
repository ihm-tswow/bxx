#pragma once

#include <bxx/objects/id.hpp>
#include <bxx/blender_types/blender_types.hpp>
#include <bxx/blender_types/iterables.hpp>
#include <bxx/blender_types/object.hpp>

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

    class collection_data : public blender_py_iterable<collection>
    {
    public:
        using blender_py_iterable<collection>::blender_py_iterable;
        collection create(std::string const& name);
        collection find(std::string const& name);
    };
}