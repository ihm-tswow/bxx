#pragma once

#include <bxx/objects/python_object.hpp>

namespace bxx
{
    class window_manager : public python_object
    {
    public:
        using python_object::python_object;

        template <typename ...Args>
        python_object invoke_props_dialog(python_object self, Args&&...args)
        {
            return call("invoke_props_dialog", self, args...);
        }
    };
}