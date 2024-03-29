#pragma once

#include <iterator>

#include <bxx/objects/blender_struct.hpp>
#include <bxx/string_literal.hpp>

namespace bxx
{
    template <
        typename blender_container,
        typename blender_ptr,
        typename bxx_type,
        size_t(*_len)(blender_container*),
        blender_ptr*(*_get)(blender_container*,size_t)
    >
    class blender_ptr_iterable : public blender_struct<blender_container>
    {
        struct iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = bxx_type;
            bxx_type operator*() const { return bxx_type(*m_ptr); }
            iterator(blender_ptr* ptr) : m_ptr(ptr) {}
            bxx_type operator*() { return bxx_type(m_ptr); }
            iterator& operator++() { m_ptr++; return *this; }
            iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
            friend bool operator== (const iterator& a, const iterator& b) { return a.m_ptr == b.m_ptr; }
            friend bool operator!= (const iterator& a, const iterator& b) { return a.m_ptr != b.m_ptr; }
        private:
            blender_ptr* m_ptr;
        };
    public:
        using blender_struct<blender_container>::blender_struct;
        iterator begin() { return _get(this->get_raw_struct(),0); }
        iterator end() { return _get(this->get_raw_struct(), len()); }
        size_t len() { return _len(this->get_raw_struct()); }
        bxx_type get(size_t index) { return _get(this->get_raw_struct(), index); }
    };

    template <typename T>
    class blender_py_iterable : public python_object
    {
    public:
        using python_object::python_object;
        struct iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            T operator*() const { return m_obj.get_item<T>(m_index); }
            iterator& operator++() { m_index++; return *this; }
            iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
            friend bool operator== (const iterator& a, const iterator& b) { return a.m_obj.get_pyobject() == b.m_obj.get_pyobject() && a.m_index == b.m_index; }
            friend bool operator!= (const iterator& a, const iterator& b) { return !(a == b); }
            iterator(python_object const& obj, size_t index)
                : m_obj(obj)
                , m_index(index)
            {}
        private:
            python_object m_obj;
            size_t m_index;
        };

        T get(size_t i) { return get_item<T>(i); }
        iterator begin() { return iterator(*this, 0); }
        iterator end() { return iterator(*this, len()); }
        size_t len() { return PyObject_Size(get_pyobject()); }
    };
}

#include <bxx/blender_types/iterables.ipp>