#pragma once

#include <Python.h>

#include <fmt/core.h>

#include <string>

// PYFIELD

#define PYFIELD(type,name)\
    void set_##name(type value) { this->setattr<type>(#name, value); }\
    type get_##name() const { return this->getattr<type>(#name); }

#define PYFIELD_DECL(type,name)\
    void set_##name(type value);\
    type get_##name() const;

#define PYFIELD_IMPL(cls,type,name)\
    void cls::set_##name(type value) { this->setattr<type>(#name, value); }\
    type cls::get_##name() const { return this->getattr<type>(#name); }

// PYFIELD_READ

#define PYFIELD_READ(type,name)\
    type get_##name() const { return this->getattr<type>(#name); }

#define PYFIELD_READ_DECL(type,name)\
    type get_##name() const;

#define PYFIELD_READ_IMPL(cls,type,name)\
    type cls::get_##name() const;

// PYFIELD_STRINGENUM

#define PYFIELD_STRINGENUM(type,name)\
    void set_##name(type value) { this->setattr<std::string>(#name, std::string(magic_enum::enum_name<type>(value)));}\
    type get_##name() const { return magic_enum::enum_cast<type>(this->getattr<std::string>(#name)).value(); }

#define PYFIELD_STRINGENUM_DECL(type,name)\
    void set_##name(type value);\
    type get_##name() const;

#define PYFIELD_STRINGENUM_IMPL(cls,type,name)\
    void cls::set_##name(type value) { this->setattr<std::string>(#name, std::string(magic_enum::enum_name<type>(value)));}\
    type cls::get_##name() const { return magic_enum::enum_cast<type>(this->getattr<std::string>(#name)).value(); }

// PYFIELD_STRINGENUM_READ

#define PYFIELD_STRINGENUM_READ(type,name)\
    type get_##name() const { return magic_enum::enum_cast<type>(this->getattr<std::string>(#name)).value(); }

#define PYFIELD_STRINGENUM_READ_DECL(type,name)\
    type get_##name() const;

#define PYFIELD_STRINGENUM_READ_IMPL(cls,type,name)\
    type cls::get_##name() const { return magic_enum::enum_cast<type>(this->getattr<std::string>(#name)).value(); }

namespace mathutils
{
    struct vec2;
    struct vec3;
    struct quaternion;
    struct rgba;
}

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

    class python_object_base
    {
    public:
        virtual PyObject* get_pyobject() const = 0;

        template <typename T = python_object, class ... Args>
        T call(std::string const& method, Args&&... args);

        template<typename T = python_object>
        T call(std::string const& method);

        template <typename T = python_object>
        T getattr(std::string const& arr) const;

        template <typename T>
        void setattr(std::string const& arr, T value);

        template <typename T = python_object>
        T get_item(std::string const& key) const;

        template <typename T = python_object>
        T get_item(std::uint32_t key) const;

        template <typename T>
        void set_item(std::string const& key, T value);

        void del_item(std::string const& key);

        size_t ref_count() const;
        std::string str() const;
        std::string repr() const;
        void delattr(std::string const& arr);
        bool hasattr(std::string const& arr) const;
    };

    class python_object : public python_object_base
    {
    public:
        python_object(PyObject* obj);
        python_object(python_object const& obj);
        python_object(python_object && obj) noexcept;
        python_object();
        ~python_object();
        PyObject* get_pyobject() const final;
        template <typename T>
        python_object(T value);
        python_object& operator=(python_object rhs);
        template <typename T>
        python_object& operator=(T value);
        template <typename T>
        bool is();
        operator PyObject*();
        static python_object steal(PyObject* obj);
    protected:
        // used by constructors of child classees that create new PyObjects
        python_object(details::pyobject_steal obj);
    private:
        PyObject* m_pyobject;
        void set_object(PyObject* pyobject);
        friend struct details::replace_python_object;
    };

    class python_object_weak : public python_object_base
    {
    public:
        python_object_weak(PyObject* obj);
        PyObject* get_pyobject() const final;
    private:
        PyObject* m_pyobject;
    };

    namespace details
    {
        // python reference we borrow
        struct python_tempref
        {
            python_tempref(PyObject* pyobj, bool needs_dec);
            // stupid optional move
            python_tempref(python_tempref&&) noexcept;
            PyObject* m_pyobj;
            bool m_needs_dec;
            operator PyObject* ();
            ~python_tempref();
            python_tempref(python_tempref const&) = delete;
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
        python_tempref cxx2py(mathutils::vec2 const& vec, bool theft);
        python_tempref cxx2py(mathutils::vec3 const& vec, bool theft);
        python_tempref cxx2py(mathutils::quaternion const& vec, bool theft);
        python_tempref cxx2py(mathutils::rgba const& rgba, bool theft);

        template <typename T>
        T py2cxx(PyObject* obj);
    }

    template <typename ...Args>
    python_object eval_pyobject(fmt::format_string<Args...> str, Args...);

    template <typename ...Args>
    PyObject* eval_pyobject_raw(fmt::format_string<Args...> str, Args...);
}

#include <bxx/objects/python_object.ipp>