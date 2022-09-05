#pragma once

#include <bxx/objects/python_object.hpp>

namespace bxx
{
    class python_tuple : public python_object
    {
    public:
        python_tuple();
        python_tuple(size_t size);
        python_tuple(PyObject* obj);

        template <typename T>
        void set(size_t index, T value);

        template <typename T = python_object>
        T get(size_t index);

        size_t len();
    };
}

#include <bxx/objects/python_tuple.ipp>