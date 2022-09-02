#pragma once

#include <Python.h>

#include <string>

#define PYFIELD(type,name)\
    void set_##name(type value) { setattr<type>(#name, value); }\
    type get_##name() { return getattr<type>(#name); }

#define PYFIELD_READ(type,name)\
    type get_##name() { return getattr<type>(#name); }

#define PYFIELD_STRINGENUM(type,name)\
    void set_##name(type value) { setattr<std::string>(#name, std::string(magic_enum::enum_name<type>(value)));}\
    type get_##name() { return magic_enum::enum_cast<type>(getattr<std::string>(#name)).value(); }

#define PYFIELD_STRINGENUM_READ(type,name)\
    type get_##name() { return magic_enum::enum_cast<type>(getattr<std::string>(#name)).value(); }

namespace bxx
{
    template <typename T>
    struct kwarg
    {
        std::string m_key;
        T m_value;
        kwarg(std::string const& key, T value);
    };

    class python_object;

    namespace details
    {
        // used to signal that this is a reference we want moved, not copied
        struct pyobject_steal
        {
            PyObject* obj;
            operator PyObject* () = delete;
        };

        // special hack class because i don't know how to reliably resolve constructors without it
        struct replace_python_object
        {
            replace_python_object(python_object& obj, PyObject* pyobj);
        };
    }

    class python_object
    {
    public:
        python_object(PyObject* obj);
        python_object(python_object const& obj);
        python_object(python_object && obj) noexcept;
        python_object();
        template <typename T>
        python_object(T value);
        python_object& operator=(python_object const& rhs);
        template <typename T>
        python_object& operator=(T value);

        ~python_object();

        PyObject* get_pyobject() const;

        template <typename T, class ... Args>
        T call(std::string const& method, Args&&... args) const;

        template<typename T>
        T call(std::string const& method) const;

        template <typename T>
        T getattr(std::string const& arr) const;

        template <typename T>
        void setattr(std::string const& arr, T value);

        template <typename T>
        bool is();

        size_t ref_count() const;
        std::string str() const;
        std::string repr() const;
        void delattr(std::string const& arr);
        bool hasattr(std::string const& arr) const;

        operator PyObject* ();
    protected:
        // used by constructors of child classees that create new PyObjects
        python_object(details::pyobject_steal obj);
    private:
        PyObject* m_pyobject;
        void set_object(PyObject* pyobject);
        friend struct details::replace_python_object;
    };

    namespace details
    {
        // python reference we borrow
        struct python_tempref
        {
            python_tempref(PyObject* pyobj, bool needs_dec);
            PyObject* m_pyobj;
            bool m_needs_dec;
            operator PyObject* ();
            ~python_tempref();
            python_tempref(python_tempref const&) = delete;
            // stupid optional move
            python_tempref(python_tempref &&);
        };

        // functions for creating pyreferences respecting reference theft
        python_tempref cxx2py(std::string const& value, bool theft);
        python_tempref cxx2py(char const* value, bool theft);
        python_tempref cxx2py(std::uint32_t value, bool theft);
        python_tempref cxx2py(std::int32_t value, bool theft);
        python_tempref cxx2py(std::int64_t value, bool theft);
        python_tempref cxx2py(std::uint64_t value, bool theft);
        python_tempref cxx2py(double value, bool theft);
        python_tempref cxx2py(float value, bool theft);
        python_tempref cxx2py(PyObject* value, bool theft);

        template <typename T>
        T py2cxx(PyObject* obj);
    }

    python_object eval_pyobject(std::string const& python);
    python_object eval_pyobject(std::initializer_list<std::string> const& python);
}

#include <bxx/objects/python_object.ipp>