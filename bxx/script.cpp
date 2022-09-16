#include <bxx/script.hpp>
#include <bxx/objects/python_object.hpp>
#include <bxx/objects/python_tuple.hpp>
#include <common/exec.hpp>
#include <core/core_defines.hpp>

std::vector<std::function<bxx::python_object(bxx::python_tuple)>> events;
size_t bxx::lib_register_event(std::function<python_object(bxx::python_tuple)> evt)
{
    events.push_back(evt);
    return events.size() - 1;
}

BXX_API PyObject* lib_fire_event(size_t index, PyObject* raw)
{
    try
    {
        bxx::python_object res = events[index](bxx::python_tuple(raw));
        PyObject* raw_res = res.get_pyobject();
        Py_IncRef(raw_res);
        return raw_res;
    }
    catch (std::exception const& err)
    {
        std::cout << err.what() << "\n";
        return nullptr;
    }
    catch (...)
    {
        std::cout << "Unknown error when firing cxx event\n";
        return nullptr;
    }
}

namespace bxx
{
    void register_cxx_function(std::string const& name, std::function<python_object(python_tuple args)> callback)
    {
        size_t event_id = lib_register_event(callback);
        exec("register_cxx_function('{}', {}, {})", name.c_str(), get_script_index(), event_id);
    }
}
