#pragma once

#include <bxx/objects/python_object.hpp>

namespace bxx
{
    template <typename blender_type>
    class blender_struct
    {
    public:
        blender_struct(blender_type* ptr);
        blender_type* get_raw_struct() const;
        blender_type* get_raw_struct();
    private:
        blender_type* m_ptr;
    };

    template <typename blender_type>
    class blender_py_struct
        : public blender_struct<blender_type>
        , public python_object_base<python_object>
    {
    public:
        using blender_struct<blender_type>::blender_struct;
        blender_py_struct(PyObject* obj);
        blender_py_struct(python_object obj);
    };
}

#include <bxx/objects/blender_struct.ipp>