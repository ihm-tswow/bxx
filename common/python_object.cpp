#include "python_object.hpp"

namespace bxx
{
    python_object::operator long() const { return PyLong_AsLong(m_obj); }
    python_object::operator bool() const { return PyObject_IsTrue(m_obj) ? true : false; }
    python_object::operator double() const { return PyFloat_AsDouble(m_obj); }
    python_object::operator std::string() const { return std::string(PyBytes_AS_STRING(m_obj)); }
    python_object::operator python_object() { return *this; }
}