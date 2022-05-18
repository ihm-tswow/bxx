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

#include "shared_functions.h"
#include "operator.h"
#include "nlohmann/json.hpp"

#include <iostream>

extern "C" {
    __declspec(dllexport) void _script_register(char const* script_name, shared_functions* functions);
    __declspec(dllexport) void _script_unregister();
    __declspec(dllexport) void _fire_operator(char const* op, char const* json);
}

void __set_script_name(std::string const& script_name);
void __init_pointers_store(shared_functions* functions);

void script_register();
void script_unregister();

void _script_register(char const* script_name, shared_functions* functions)
{
    __init_pointers_store(functions);
    __set_script_name(script_name);
    script_register();
}

void _script_unregister()
{
    script_unregister();
}

void _fire_operator(char const* op, char const* json)
{
    auto callback = bxx::operators::get_callback(op);
    if (callback == nullptr)
    {
        std::cout << "Error: Operator " << op << " has no callback registered\n";
        return;
    }
    callback(nlohmann::json::parse(json));
}
