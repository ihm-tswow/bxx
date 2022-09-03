#include <bxx/objects/python_tuple.hpp>

namespace bxx
{
    python_tuple::python_tuple(size_t size)
        : python_object(details::pyobject_steal(PyTuple_New(size)))
    {}

    python_tuple::python_tuple(PyObject* obj)
        : python_object(obj)
    {}

    python_tuple::python_tuple()
        : python_object(details::pyobject_steal(PyTuple_New(0)))
    {}

    size_t python_tuple::len()
    {
        return PyTuple_Size(get_pyobject());
    }
}