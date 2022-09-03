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
        std::string get_name_full() const;
        std::string get_name() const;
        virtual std::string get_type_path() const = 0;
        python_object get_pyobject() final;
    protected:
    };
}

#include <bxx/objects/id.ipp>