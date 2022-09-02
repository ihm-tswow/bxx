#include <bxx/objects/python_object.hpp>
#include <common/exec.hpp>

namespace bxx
{
    python_object::python_object(details::pyobject_steal obj)
        : m_pyobject(obj.obj)
    {
    }

    python_object::python_object(python_object&& obj) noexcept
        : m_pyobject(obj.get_pyobject())
    {
        obj.m_pyobject = nullptr;
    }

    python_object::python_object()
        : m_pyobject(Py_None)
    {
        Py_IncRef(m_pyobject);
    }

    python_object::python_object(PyObject* obj)
        : m_pyobject(obj)
    {
        Py_IncRef(m_pyobject);
    }

    python_object::python_object(python_object const& obj)
        : m_pyobject(obj.get_pyobject())
    {
        Py_IncRef(m_pyobject);
    }

    python_object::~python_object()
    {
        if (m_pyobject)
        {
            Py_DecRef(m_pyobject);
        }
    }

    size_t python_object::ref_count() const
    {
        return Py_REFCNT(get_pyobject());
    }

    PyObject* python_object::get_pyobject() const
    {
        return m_pyobject;
    }

    std::string python_object::str() const
    {
        PyObject* str = PyObject_Str(get_pyobject());
        if (!str)
        {
            return "<error str>";
        }
        char const* chr = _PyUnicode_AsString(str);
        Py_DecRef(str);
        return std::string(chr);
    }

    std::string python_object::repr() const
    {
        PyObject* str = PyObject_Repr(get_pyobject());
        if (!str)
        {
            return "<error repr>";
        }
        char const* chr = _PyUnicode_AsString(str);
        Py_DecRef(str);
        return std::string(chr);
    }

    void python_object::delattr(std::string const& arr)
    {
        PyObject_DelAttrString(get_pyobject(), arr.c_str());
    }

    bool python_object::hasattr(std::string const& arr) const
    {
        return PyObject_HasAttrString(get_pyobject(), arr.c_str());
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

    python_object eval_pyobject(std::string const& python)
    {
        PyObject* obj = get_pointers()->cy_eval_pyobject(const_cast<char*>(python.c_str()));
        return python_object(obj);
    }

    python_object eval_pyobject(std::initializer_list<std::string> const& python)
    {
        return eval_pyobject(join_strings(python));
    }

    python_object& python_object::operator=(python_object const& rhs)
    {
        set_object(rhs.m_pyobject);
        return *this;
    }

    namespace details
    {
        python_tempref::operator PyObject* ()
        {
            return m_pyobj;
        }

        python_tempref::python_tempref(python_tempref&& orig)
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

        replace_python_object::replace_python_object(python_object& obj, PyObject* pyobj)
        {
            obj.set_object(pyobj);
        }
    }
}