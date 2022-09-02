#pragma once

#include <bxx/objects/python_object.hpp>

namespace bxx
{
    template <typename pointer_type>
    class id : public python_object
    {
    public:
        id(PyObject* obj);
        std::string get_full_name() const;
        std::string get_name() const;
        pointer_type* get_pointer() const;
        pointer_type* get_pointer();
    private:
        pointer_type* m_ptr;
    };
}

#include <bxx/objects/id.ipp>