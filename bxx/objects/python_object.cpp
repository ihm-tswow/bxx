#include <bxx/objects/python_object.hpp>
#include <bxx/mathutils.hpp>
#include <common/script_error.hpp>
#include <common/exec.hpp>

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

    python_object python_object::steal(PyObject* obj)
    {
        return python_object(details::pyobject_steal(obj));
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

    PyObject* python_object_weak::get_pyobject() const
    {
        return m_pyobject;
    }

    PyObject* python_object::get_pyobject() const
    {
        return m_pyobject;
    }

    size_t python_object_base::ref_count() const
    {
        if (!get_pyobject())
        {
            return 0;
        }
        return Py_REFCNT(get_pyobject());
    }

    std::string python_object_base::str() const
    {
        if (!get_pyobject())
        {
            return "null";
        }
        python_object str = python_object::steal(PyObject_Str(get_pyobject()));
        BXX_SCRIPT_ASSERT(str.get_pyobject(), internal_python_error, "Failed to str() python object");
        char const* chr = _PyUnicode_AsString(str);
        return std::string(chr);
    }

    std::string python_object_base::repr() const
    {
        if (!get_pyobject())
        {
            return "null";
        }
        python_object str = python_object::steal(PyObject_Repr(get_pyobject()));
        BXX_SCRIPT_ASSERT(str, internal_python_error,"Failed to repr() python object");
        char const* chr = _PyUnicode_AsString(str);
        Py_DecRef(str);
        return std::string(chr);
    }

    std::string python_object_base::type_str() const
    {
        if (!get_pyobject())
        {
            return "std::nullptr_t";
        }
        python_object type = python_object::steal(PyObject_Type(get_pyobject()));
        python_object str = python_object::steal(PyObject_Str(type.get_pyobject()));
        char const* chr = _PyUnicode_AsString(str);
        return std::string(chr);
    }


    void python_object_base::delattr(std::string const& arr)
    {
        BXX_SCRIPT_ASSERT(is_valid(), python_object_error, "tried to delete attribute {} on null python_object",arr.c_str());
        BXX_SCRIPT_ASSERT(hasattr(arr), python_key_error, "tried to delete missing attribute {}",arr.c_str());
        int err = PyObject_DelAttrString(get_pyobject(), arr.c_str());
        BXX_SCRIPT_ASSERT(!err, python_key_error, "failed to delete python attribute {}", arr.c_str());
    }

    bool python_object_base::hasattr(std::string const& arr) const
    {
        BXX_SCRIPT_ASSERT(is_valid(), python_object_error, "tried to read attribute on null python_object");
        return PyObject_HasAttrString(get_pyobject(), arr.c_str());
    }

    void python_object_base::del_item(std::string const& key)
    {
        BXX_SCRIPT_ASSERT(is_valid(), python_object_error, "tried to delete item on null python_object");
        PyObject_DelItemString(get_pyobject(), key.c_str());
    }

    bool python_object_base::is_valid() const
    {
        return get_pyobject() && get_pyobject() != Py_None;
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
        {
            BXX_SCRIPT_ASSERT(pyobj, internal_python_error, "failed to create python stack variable");
        }

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
            BXX_SCRIPT_ASSERT(value, python_object_error, "tried to convert null PyObject");
            if (theft)
            {
                Py_IncRef(value);
            }
            return { value , false };
        }

        python_tempref cxx2py(python_object value, bool theft)
        {
            BXX_SCRIPT_ASSERT(value.get_pyobject(), python_object_error, "tried to convert null python_object");
            if (theft)
            {
                Py_IncRef(value.get_pyobject());
            }
            return { value.get_pyobject() , false};
        }

        python_tempref cxx2py(mathutils::vec2 const& vec, bool theft)
        {
            PyObject* tup = PyTuple_New(2);
            BXX_SCRIPT_ASSERT(tup, internal_python_error, "failed to create PyTuple");
            PyTuple_SetItem(tup, 0, PyFloat_FromDouble(vec.x));
            PyTuple_SetItem(tup, 1, PyFloat_FromDouble(vec.y));
            return { tup, !theft };
        }

        python_tempref cxx2py(mathutils::vec3 const& vec, bool theft)
        {
            PyObject* tup = PyTuple_New(3);
            BXX_SCRIPT_ASSERT(tup, internal_python_error, "failed to create PyTuple");
            PyTuple_SetItem(tup, 0, PyFloat_FromDouble(vec.x));
            PyTuple_SetItem(tup, 1, PyFloat_FromDouble(vec.y));
            PyTuple_SetItem(tup, 2, PyFloat_FromDouble(vec.z));
            return { tup, !theft };
        }

        python_tempref cxx2py(mathutils::quaternion const& vec, bool theft)
        {
            PyObject* tup = PyTuple_New(4);
            BXX_SCRIPT_ASSERT(tup, internal_python_error, "failed to create PyTuple");
            PyTuple_SetItem(tup, 0, PyFloat_FromDouble(vec.w));
            PyTuple_SetItem(tup, 1, PyFloat_FromDouble(vec.x));
            PyTuple_SetItem(tup, 2, PyFloat_FromDouble(vec.y));
            PyTuple_SetItem(tup, 3, PyFloat_FromDouble(vec.z));
            return { tup, !theft };
        }

        python_tempref cxx2py(mathutils::rgba const& rgba, bool theft)
        {
            PyObject* tup = PyTuple_New(4);
            BXX_SCRIPT_ASSERT(tup, internal_python_error, "failed to create PyTuple");
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