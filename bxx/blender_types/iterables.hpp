#pragma once

#include <iterator>

#include <bxx/objects/blender_struct.hpp>

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
}

#include <bxx/blender_types/iterables.hpp>