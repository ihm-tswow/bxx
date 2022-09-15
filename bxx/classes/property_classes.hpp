#pragma once

#include <bxx/objects/python_object.hpp>
#include <bxx/objects/python_set.hpp>
#include <bxx/builders/property_builder.hpp>
#include <bxx/string_literal.hpp>
#include <bxx/enums.hpp>

#include <limits>
#include <vector>

#define PROPERTY_ENTRY(prop) prop(this, #prop)

namespace bxx
{
    // hack of a class needed to get rid of the template
    class class_property_builder : public property_builder<class_property_builder> {};

    class property_class;
    template <typename T>
    class operator_class;

    class property_base
    {
    public:
        property_base(property_class* owner, char const* id);
        python_object get_python_object();
        std::string get_id() const { return m_id; }
        python_object get_owner() const;
        virtual void write_to(class_property_builder& builder) = 0;
    protected:
        friend class property_class;
        char const* m_id;
        property_class* m_owner;
    };

    class property_class : public python_object
    {
    public:
        property_class() = default;
        property_class(property_class const&) = delete;
        property_class(PyObject* obj)
            : python_object(obj)
        {}
    protected:
        virtual void register_class_internal() = 0;
        virtual std::string get_class_name() = 0;
        std::vector<property_base*> m_properties;
        friend class property_base;
    };

    inline python_object property_base::get_python_object()
    {
        return m_owner->getattr<python_object>(m_id);
    }

    inline property_base::property_base(property_class* owner, char const* id)
        : m_owner(owner)
        , m_id(id)
    {
        m_owner->m_properties.push_back(this);
    }

    inline python_object property_base::get_owner() const
    {
        return python_object(m_owner->get_pyobject());
    }

    template <
        string_literal name,
        bool def = false,
        string_literal description = ""
    >
    class bool_property : public property_base
    {
    public:
        bool_property(property_class* owner, char const* id)
            : property_base(owner, id)
        {}

        bool get_value()
        {
            return m_owner->getattr<bool>(m_id);
        }
    protected:
        void write_to(class_property_builder& builder) final
        {
            builder.add_bool_property(m_id, name.value, def, description.value);
        }
    };

    template <
        string_literal name,
        double def = 0.0,
        double min = double(std::numeric_limits<std::int32_t>::min()),
        double max = double(std::numeric_limits<std::int32_t>::max())
    >
    class float_property : public property_base
    {
    public:
        float_property(property_class* owner, char const* id)
            : property_base(owner, id)
        {}

        double get_value()
        {
            return m_owner->getattr<double>(m_id);
        }
    protected:
        void write_to(class_property_builder& builder) final
        {
            builder.add_float_property(m_id, name.value, -1007688, def, 1007688);
        }
    };

    template <
        string_literal name,
        std::int64_t def = 0,
        std::int64_t min = std::numeric_limits<std::int64_t>::min(),
        std::int64_t max = std::int64_t(std::numeric_limits<std::int64_t>::max()),
        string_literal description = ""
    >
    class int_property : public property_base
    {
    public:
        int_property(property_class* owner, char const* id)
            : property_base(owner, id)
        {}

        std::int64_t get_value()
        {
            return m_owner->getattr<double>(m_id);
        }
    protected:
        void write_to(class_property_builder& builder) final
        {
            builder.add_float_property(m_id, name.value, min, def,max, description.value);
        }
    };

    template <
        string_literal name,
        std::vector<enum_entry>(*entry_producer)(python_object,python_object),
        string_literal description = ""
    >
    class dynamic_enum_property : public property_base
    {
    public:
        dynamic_enum_property(property_class* owner, char const* id)
            : property_base(owner,id)
        {}

        std::string get_value()
        {
            return m_owner->getattr<std::string>(m_id);
        }
    private:
        void write_to(class_property_builder& builder) final
        {
            builder.add_dynamic_enum_property(m_id, name.value, [&](python_object o1, python_object o2) { return entry_producer(o1,o2); }, description.value);
        }
    };

    template <
        typename T,
        string_literal name,
        T def,
        string_literal description = "",
        enum_meta(*meta_producer)(T) = enums::get_enum_meta<T>
    >
    class enum_property : public property_base
    {
    public:
        enum_property(property_class* owner, char const* id)
            : property_base(owner,id)
        {}

        T get_value()
        {
            PyObject* attr = PyObject_GetAttrString(m_owner->get_pyobject(), m_id);
            char const* c = _PyUnicode_AsString(attr);
            return enums::get_enum_value<T>(c);
        }

    protected:
        void write_to(class_property_builder& builder) final
        {
            std::vector<std::pair<T,std::string>> entries = enums::get_enum_entries<T>();
            std::vector<enum_entry> copy;
            copy.reserve(entries.size());
            for (auto const& [entryVal,entryName] : entries)
            {
                enum_meta meta = meta_producer ? meta_producer(entryVal) : enum_meta(entryName);
                copy.push_back(enum_entry(
                    entryName,
                    meta.m_name,
                    meta.m_description,
                    meta.m_icon
                ));
            }
            builder.add_enum_property(m_id, name.value, copy, description.value);
        }
    };

    template <
        typename T,
        string_literal name,
        string_literal description = "",
        enum_meta(*meta_producer)(T) = enums::get_enum_meta<T>
    >
    class mask_property : public property_base
    {
    public:
        mask_property(property_class* owner, char const* id)
            : property_base(owner, id)
        {}

        std::uint64_t get_value()
        {
            std::uint64_t value = 0;
            python_object set = python_object::steal(PyObject_GetAttrString(m_owner->get_pyobject(), m_id));
            set.for_each<python_object>([&](python_object obj) {
                if (obj.is<std::string>())
                {
                    std::string str = details::py2cxx<std::string>(obj);
                    T num = enums::get_enum_value<T>(str).value();
                    value |= static_cast<std::uint64_t>(num);
                }
            });
            return value;
        }

    protected:

        void write_to(class_property_builder& builder) final
        {
            std::vector<std::pair<T, std::string>> entries = enums::get_enum_entries<T>();
            std::vector<enum_entry> copy;
            copy.reserve(entries.size());
            for (auto const& [entryVal, entryName] : entries)
            {
                enum_meta meta = meta_producer ? meta_producer(entryVal) : enum_meta(entryName);
                copy.push_back(enum_entry(
                    entryName,
                    meta.m_name,
                    meta.m_description,
                    meta.m_icon
                ));
            }
            builder.add_mask_property(m_id, name.value, {}, copy, description.value);
        }
    };
}
