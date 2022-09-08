// =============================================================
//
// - INTERNAL FILE - DO NOT EDIT -
//
//   For devs:
//
//   This file is a hack to make users script boilerplate as
//   minimal as possible, as we still have to pass the cython
//   pointers to our scripts somehow. The solution here is to
//   middleman the script entrypoints and set up blender_cxx
//   before the scripts themselves are called.
//
//   Redesigning the script loading to use shared libraries
//   can remove the need for this, and doing it like this now
//   at least ensures api stability for that change.
//
// =============================================================

#include <core/core_defines.hpp>
#include <bxx/objects/python_object.hpp>
#include <bxx/objects/python_tuple.hpp>
#include <common/shared_functions.hpp>
#include <common/addon.hpp>

#include <nlohmann/json.hpp>

// Private functions
void register_operators();
void init_pointers_store(shared_functions* functions);
void setup_script_data(std::string const& name, size_t index);
extern std::vector<std::function<bxx::python_object(bxx::python_tuple)>> events;
extern std::string cached_library_path; // script_error.cpp

// Provided by user
void script_register();
void script_unregister();

// Called from core
BXX_API void lib_script_register(char const* script_name, size_t index, shared_functions* functions)
{
    try {
        init_pointers_store(functions);
        cached_library_path = (bxx::get_addon_path() / "lib").string();
        setup_script_data(script_name, index);
        register_operators();
        script_register();
    }
    catch (std::exception const& err)
    {
        std::cout << err.what() << "\n";
    }
    catch (...)
    {
        std::cout << "Unknown error when registering script " << script_name << "\n";
    }
}

BXX_API void lib_script_unregister()
{
    try
    {
        script_unregister();
    }
    catch (std::exception const& err)
    {
        std::cout << err.what() << "\n";
    }
    catch (...)
    {
        std::cout << "Unknown error when unregistering script\n";
    }
}
