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

#include "shared_functions.hpp"
#include "builders/operator_builder.hpp"
#include "nlohmann/json.hpp"
#include "core_defines.hpp"

#include <iostream>


// Private functions
void register_operators();
void init_pointers_store(shared_functions* functions);
void setup_script_data(std::string const& name, size_t index);
extern std::vector<std::function<PyObject*(PyObject*)>> events;

// Provided by user
void script_register();
void script_unregister();

// Called from core
BXX_API void lib_script_register(char const* script_name, size_t index, shared_functions* functions)
{
    init_pointers_store(functions);
    setup_script_data(script_name, index);
    register_operators();
    script_register();
}

BXX_API void lib_script_unregister()
{
    script_unregister();
}

BXX_API PyObject* lib_fire_event(size_t index, PyObject* obj)
{
    return events[index](obj);
}
