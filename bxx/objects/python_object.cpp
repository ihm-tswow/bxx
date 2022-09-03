#include <bxx/objects/python_object.hpp>
#include <common/exec.hpp>
#include <bxx/mathutils.hpp>

namespace bxx
{
    // steal
    python_object::python_object(details::pyobject_steal obj)
        : m_pyobject(obj.obj)
    {
    }

    // steal
    python_object::python_object(python_object&& obj) noexcept
        : m_pyobject(obj.get_pyobject())
    {
        obj.m_pyobject = nullptr;
    }

    // null init
    python_object::python_object()
        : m_pyobject(Py_None)
    {}

    // copy
    python_object::python_object(PyObject* obj)
        : m_pyobject(obj)
    {
        if (obj)
        {
            Py_IncRef(m_pyobject);
        }
    }

    // copy
    python_object::python_object(python_object const& obj)
        : python_object(obj.m_pyobject)
    {
    }

    python_object::~python_object()
    {
        if (m_pyobject)
        {
            Py_DecRef(m_pyobject);
        }
    }

    void python_object::set_object(PyObject* pyobject)
    {
        if (m_pyobject == pyobject)
        {
            return;
        }
        if (m_pyobject)
        {
            Py_DecRef(m_pyobject);
        }
        m_pyobject = pyobject;
        Py_IncRef(m_pyobject);
    }

    python_object eval_pyobject(std::string const& python)
    {
        PyObject* obj = get_pointers()->cy_eval_pyobject(const_cast<char*>(python.c_str()));
        return python_object(obj);
    }

    python_object eval_pyobject(std::initializer_list<std::string> const& python)
    {
        return eval_pyobject(join_strings(python));
    }

    python_object::operator PyObject* ()
    {
        return get_pyobject();
    }

    python_object& python_object::operator=(python_object rhs)
    {
        set_object(rhs.m_pyobject);
        return *this;
    }

    python_object_weak::python_object_weak(PyObject* obj)
        : m_pyobject(obj)
    {}

    PyObject* python_object_weak::get_pyobject()
    {
        return m_pyobject;
    }

    PyObject* python_object::get_pyobject()
    {
        return m_pyobject;
    }

    namespace details
    {
        python_tempref::operator PyObject* ()
        {
            return m_pyobj;
        }

        python_tempref::python_tempref(python_tempref&& orig) noexcept
        {
            m_needs_dec = orig.m_needs_dec;
            m_pyobj = orig.m_pyobj;
            orig.m_needs_dec = false;
        }

        python_tempref::~python_tempref()
        {
            if (m_needs_dec)
            {
                Py_DecRef(m_pyobj);
            }
        }

        python_tempref::python_tempref(PyObject* pyobj, bool needs_dec)
            : m_pyobj(pyobj)
            , m_needs_dec(needs_dec)
        {}

        python_tempref cxx2py(std::string const& value, bool theft)
        {
            return { PyUnicode_FromString(value.c_str()) , !theft};
        }

        python_tempref cxx2py(char const* value, bool theft)
        {
            return { PyUnicode_FromString(value) , !theft};
        }

        python_tempref cxx2py(std::uint32_t value, bool theft)
        {
            return { PyLong_FromUnsignedLong(value) , !theft};
        }

        python_tempref cxx2py(std::int32_t value, bool theft)
        {
            return { PyLong_FromLong(value) , !theft};
        }

        python_tempref cxx2py(std::int64_t value, bool theft)
        {
            return { PyLong_FromLongLong(value) , !theft};
        }

        python_tempref cxx2py(std::uint64_t value, bool theft)
        {
            return { PyLong_FromUnsignedLongLong(value) , !theft};
        }

        python_tempref cxx2py(double value, bool theft)
        {
            return { PyFloat_FromDouble(value) , !theft};
        }

        python_tempref cxx2py(float value, bool theft)
        {
            return { PyFloat_FromDouble(value) , !theft};
        }

        python_tempref cxx2py(PyObject* value, bool theft)
        {
            if (theft)
            {
                Py_IncRef(value);
            }
            return { value , false };
        }

        python_tempref cxx2py(mathutils::vec2 const& vec, bool theft)
        {
            PyObject* tup = PyTuple_New(2);
            PyTuple_SetItem(tup, 0, PyFloat_FromDouble(vec.x));
            PyTuple_SetItem(tup, 1, PyFloat_FromDouble(vec.y));
            return { tup, !theft };
        }

        python_tempref cxx2py(mathutils::vec3 const& vec, bool theft)
        {
            PyObject* tup = PyTuple_New(3);
            PyTuple_SetItem(tup, 0, PyFloat_FromDouble(vec.x));
            PyTuple_SetItem(tup, 1, PyFloat_FromDouble(vec.y));
            PyTuple_SetItem(tup, 2, PyFloat_FromDouble(vec.z));
            return { tup, !theft };
        }

        python_tempref cxx2py(mathutils::quaternion const& vec, bool theft)
        {
            PyObject* tup = PyTuple_New(4);
            PyTuple_SetItem(tup, 0, PyFloat_FromDouble(vec.w));
            PyTuple_SetItem(tup, 1, PyFloat_FromDouble(vec.x));
            PyTuple_SetItem(tup, 2, PyFloat_FromDouble(vec.y));
            PyTuple_SetItem(tup, 3, PyFloat_FromDouble(vec.z));
            return { tup, !theft };
        }

        python_tempref cxx2py(mathutils::rgba const& rgba, bool theft)
        {
            PyObject* tup = PyTuple_New(4);
            PyTuple_SetItem(tup, 0, PyFloat_FromDouble(rgba.r));
            PyTuple_SetItem(tup, 1, PyFloat_FromDouble(rgba.g));
            PyTuple_SetItem(tup, 2, PyFloat_FromDouble(rgba.b));
            PyTuple_SetItem(tup, 3, PyFloat_FromDouble(rgba.a));
            return { tup, !theft };
        }

        replace_python_object::replace_python_object(python_object& obj, PyObject* pyobj)
        {
            obj.set_object(pyobj);
        }
    }
}