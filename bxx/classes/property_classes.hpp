#pragma once

#include "python_object.hpp"
#include "../builders/property_builder.hpp"
#include "magic_enum.hpp"

#include <limits>
#include <vector>

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
    protected:
        virtual void write_to(class_property_builder& builder) = 0;
        friend class property_class;

        template<class T>
        friend class operator_class;

        char const* m_id;
        property_class* m_owner;
    };

    class property_class : public python_object
    {
    public:
        property_class(property_class const& oth) = delete;
        property_class() {};
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
        return python_object(m_owner->m_obj);
    }

    template<size_t N>
    struct string_literal {
        constexpr string_literal(const char(&str)[N]) {
            std::copy_n(str, N, value);
        }
        char value[N];
    };

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

    struct enum_meta
    {
        std::string m_name;
        std::string m_description = "";
        std::string m_icon = "";
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
        enum_meta(*meta_producer)(T) = nullptr
    >
    class enum_property : public property_base
    {
    public:
        enum_property(property_class* owner, char const* id)
            : property_base(owner,id)
        {}

        T get_value()
        {
            PyObject* attr = PyObject_GetAttrString(m_owner->m_obj,m_id);
            char const* c = _PyUnicode_AsString(attr);
            return magic_enum::enum_cast<T>(c).value();
        }

    protected:
        void write_to(class_property_builder& builder) final
        {
            auto enums = magic_enum::enum_values<T>();
            std::vector<enum_entry> copy;
            for (size_t i = 0; i < enums.size(); ++i)
            {
                std::string name_entry(magic_enum::enum_name<T>(enums[i]));
                enum_meta meta = meta_producer ? meta_producer(enums[i]) : enum_meta(name_entry);
                copy.push_back(enum_entry(
                    name_entry,
                    meta.m_name,
                    meta.m_description,
                    meta.m_icon
                ));
            }
            builder.add_enum_property(m_id, name.value, copy, description.value);
        }
    };

#define PROPERTY_CLASS(cls,...) static void register_class() { cls().register_class_internal(); } std::string get_class_name() { return #cls; } cls(): __VA_ARGS__ {}
#define PROPERTY_ENTRY(prop) prop(this, #prop)
}
