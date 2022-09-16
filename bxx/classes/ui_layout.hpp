#pragma once

#include <bxx/objects/python_object.hpp>
#include <bxx/classes/property_classes.hpp>

namespace bxx
{
    /*
    enum class alignment
    {
        EXPAND,
        LEFT,
        CENTER,
        RIGHT
    };
    */

    enum class direction
    {
        HORIZONTAL,
        VERTICAL
    };

    class ui_layout : public python_object
    {
    public:
        ui_layout(PyObject* obj)
            : python_object(obj)
        {}
        PYFIELD(bool, active)
        PYFIELD(bool, enabled)

        //PYFIELD_STRINGENUM(alignment, alignment)
        PYFIELD_STRINGENUM(direction, direction)

        template <typename ...Args>
        void prop(property_base const& base, Args&&... args)
        {
            call<python_object>("prop", base.get_owner(), base.get_id(), args...);
        }
        template <typename ...Args>
        void label(std::string const& text, Args&&... args)
        {
            call<python_object>("label", kwarg("text", text), args...);
        }

        template <typename col_prop_type, typename active_prop_type, typename ...Args>
        void template_list(std::string const& name, col_prop_type col, active_prop_type active, Args&&...args)
        {
            call("template_list", name, "", col.get_owner(), col.get_id(), active.get_owner(), active.get_id(), args...);
        }
    };
}