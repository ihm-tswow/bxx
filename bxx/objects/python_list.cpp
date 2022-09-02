#include <bxx/objects/python_list.hpp>

namespace bxx
{
    python_list::python_list(size_t size)
        : python_object(details::pyobject_steal(PyList_New(size)))
    {}

    python_list::python_list(PyObject* obj)
        : python_object(obj)
    {}

    python_list::python_list()
        : python_object(details::pyobject_steal(PyList_New(0)))
    {}

    size_t python_list::len()
    {
        return PyList_Size(get_pyobject());
    }
}