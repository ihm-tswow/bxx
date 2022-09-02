#include <bxx/objects/python_dict.hpp>

namespace bxx
{
    template <typename T>
    void python_dict::set(std::string const& key, T value)
    {
        PyDict_SetItemString(get_pyobject(), key.c_str(), details::cxx2py(value, false));
    }

    template <typename T>
    T python_dict::get(std::string const& key)
    {
        PyObject* obj = PyDict_GetItemString(get_pyobject(), key.c_str());
        T value(details::py2cxx<T>(obj));
        return value;
    }
}