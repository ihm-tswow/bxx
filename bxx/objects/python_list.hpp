#pragma once

#include <bxx/objects/python_object.hpp>

namespace bxx
{
    class python_list: public python_object
    {
    public:
        python_list(size_t size);
        python_list(PyObject* obj);
        python_list();

        template <typename T>
        void set(size_t index, T value);

        template <typename T>
        T get(size_t index);

        template <typename T>
        void append(T value);

        size_t len();
    };
}

#include <bxx/objects/python_list.ipp>