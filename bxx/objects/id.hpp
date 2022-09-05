#pragma once

#include <bxx/objects/python_object.hpp>
#include <bxx/objects/blender_struct.hpp>

namespace bxx
{
    template <typename pointer_type>
    class id : public blender_py_struct<pointer_type>
    {
    public:
        using blender_py_struct<pointer_type>::blender_py_struct;
        PYFIELD(std::string,name)
        PYFIELD(std::string,name_full)
    };
}

#include <bxx/objects/id.ipp>