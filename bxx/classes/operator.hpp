#pragma once

#include <bxx/blender_types/context.hpp>
#include <bxx/classes/property_classes.hpp>
#include <bxx/classes/ui_layout.hpp>
#include <bxx/builders/operator_builder.hpp>

#define OPERATOR(cls,...)\
    static void register_class()\
    {\
        cls().register_class_internal();\
    }\
    std::string get_class_name()\
    {\
        return #cls;\
    }\
    cls(): __VA_ARGS__ {}\
    cls(PyObject* obj)\
        : __VA_ARGS__\
    {\
        bxx::details::replace_python_object(*this, obj);\
    }

#define OPERATOR_NO_PROPS(cls)\
    static void register_class()\
    {\
        cls().register_class_internal();\
    }\
    std::string get_class_name()\
    {\
        return #cls;\
    }\
    cls() {}\
    cls(PyObject* obj)\
    {\
        bxx::details::replace_python_object(*this,obj);\
    }\

#define SIMPLE_OPERATOR(cls,fn)\
    class cls : public bxx::operator_class<cls>\
    {\
    public:\
        void execute fn\
        OPERATOR_NO_PROPS(cls)\
    };\

namespace bxx
{
    template <typename T>
    class operator_class : public property_class
    {
    public:
        virtual void execute() = 0;

        virtual void draw() {
            for (property_base* base : m_properties)
            {
                get_layout().prop(*base);
            }
        };

        virtual python_object invoke(context ctx, python_object evt)
        {
            return python_object();
        }

        PYFIELD(ui_layout, layout);

        void register_class_internal() final
        {
            class_property_builder tmp_builder;
            for (property_base* base : m_properties)
            {
                base->write_to(tmp_builder);
            }

            operator_builder op(get_class_name(), true);
            op.read_properties_from(tmp_builder);
            op.set_callback([this](python_object obj) {
                T(obj.get_pyobject()).execute();
            });

            op.set_disable_drawing(false);
            op.set_draw([this](python_object obj) {
                T(obj.get_pyobject()).draw();
            });

            if (!std::is_same<decltype(&T::invoke), decltype(&operator_class::invoke)>::value)
            {
                op.set_invoke([this](python_object self, python_object ctx, python_object evt) {
                    return T(self.get_pyobject()).invoke(context(ctx), evt);
                });
            }
        }
    };
}