#pragma once

#include <bxx/objects/python_object.hpp>

namespace bxx
{
    class python_set : public python_object
    {
    public:
        template <typename T>
        void add(T value);

        template <typename T>
        bool contains(T value);

        size_t len();
    };
}

#include <bxx/objects/python_set.ipp>