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

        template <typename T>
        T get(size_t index) const;

        size_t len() const;
    };
}

#include <bxx/objects/python_tuple.ipp>