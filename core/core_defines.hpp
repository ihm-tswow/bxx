#pragma once

#include "../common/shared_functions.hpp"
#include "../common/tests.hpp"

typedef void (*script_register_ct)(char const* script_name, shared_functions* functions);
typedef void (*script_unregister_ct)();
typedef void (*cy_unregister_script_ct)(char*);
typedef bxx::test_collection* (*register_tests_ct)();
typedef void (*fire_operator_ct)(char* op, char* json);

#if defined(WIN32) || defined (_WIN32) || defined(__WIN32)
#include <windows.h>
typedef HINSTANCE sl_ptr_ct;
#define SL_FN GetProcAddress
#define SL_LOAD LoadLibrary
#define SL_CLOSE FreeLibrary
#define SL_EXT ".dll"
#define BXX_API extern "C" __declspec(dllexport)
#else
#include <dlfcn.h>
typedef void* sl_ptr_ct;
#define SL_FN dlsym
#define SL_LOAD(x) dlopen(x,RTLD_LAZY)
#define SL_CLOSE dlclose
#define SL_EXT ".so"
#define BXX_API extern "C" __attribute__((visibility("default")))
#endif
