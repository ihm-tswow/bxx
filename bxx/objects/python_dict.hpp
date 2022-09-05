#pragma once

#include <bxx/objects/python_object.hpp>

namespace bxx
{
    class python_list;
    class python_dict : public python_object
    {
    public:
        python_dict();
        python_dict(PyObject* obj);

        template <typename T>
        void set(std::string const& key, T value);

        template <typename T = python_object>
        T get(std::string const& key);

        void del(std::string const& key);
        python_list items();
        python_list keys();
        python_list values();
        size_t len();
    };
}

#include <bxx/objects/python_dict.ipp>