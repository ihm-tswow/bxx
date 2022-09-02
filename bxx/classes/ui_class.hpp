#pragma once

#include "ui_layout.hpp"
#include <bxx/objects/python_object.hpp>

#include <string>

#define UI_CLASS(classname)\
    classname() = default;\
    classname(python_object const& obj)\
    {\
        details::replace_python_object(*this,obj.get_pyobject());\
    }\
    \
    std::string get_class_name()\
    {\
        return #classname;\
    }\
    \
    static void register_class()\
    {\
        classname().register_internal();\
    }\

namespace bxx
{
    class ui_class : public python_object
    {
    public:
        PYFIELD(ui_layout, layout);
        virtual std::string get_class_name() = 0;
        virtual std::string display_name() { return get_class_name(); }
    protected:
        virtual void register_internal() = 0;
    };

}