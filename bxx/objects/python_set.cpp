#pragma once

#include <bxx/objects/python_set.hpp>

namespace bxx
{
    size_t python_set::len()
    {
        return PySet_Size(get_pyobject());
    }
}