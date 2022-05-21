// relative imports because cmake can't add include directories for custom targets
#include "../common/shared_functions.hpp"
#include "../common/preferences.hpp"
#include "../common/exec.hpp"
#include "../common/tests.hpp"

#include <iostream>
#include <filesystem>
#include <map>

namespace fs = std::filesystem;
typedef void (*script_register_ct)(char const* script_name, shared_functions* functions);
typedef void (*script_unregister_ct)();
typedef void (*cy_unregister_script_ct)(char*);
typedef bxx::test_collection* (*__register_tests_ct)();

static cy_unregister_script_ct _cy_unregister_script = nullptr;

#if defined(WIN32) || defined (_WIN32) || defined(__WIN32)
    #include <windows.h>
    typedef HINSTANCE sl_ptr_ct;
    #define SL_FN GetProcAddress
    #define SL_LOAD LoadLibrary
    #define SL_CLOSE FreeLibrary
    #define SL_EXT ".dll"
#else
    #include <dlfcn.h>
    typedef void* sl_ptr_ct;
    #define SL_FN dlsym
    #define SL_LOAD(x) dlopen(x,RTLD_LAZY)
    #define SL_CLOSE dlclose
    #define SL_EXT ".so"
#endif

// Functions called from cython
extern "C" {
    void setup_cxx(
        char* path,
        cy_exec_ct cy_exec,
        cy_eval_ptr_ct cy_eval_ptr,
        cy_eval_int_ct cy_eval_int,
        cy_eval_float_ct cy_eval_float,
        cy_eval_string_ct cy_eval_string,
        cy_unregister_script_ct cy_unregister_script
    );
    void register_cxx();
    void unregister_cxx();
    void auto_reload_cxx();
    void run_tests(char* include, char* exclude);
}

static shared_functions functions;
static fs::path root_path;

struct library_handle
{
    fs::file_time_type m_ctime;
    sl_ptr_ct m_library;
    std::string m_name;
    fs::path m_load_path;
};
static std::map<std::string,library_handle> libraries;

static void unload_script(library_handle const& handle)
{
    script_unregister_ct script_unregister = (script_unregister_ct)SL_FN(handle.m_library, "_script_unregister");
    if (script_unregister)
    {
        script_unregister();
    }
    _cy_unregister_script(const_cast<char*>(handle.m_name.c_str()));
    SL_CLOSE(handle.m_library);
    fs::remove(handle.m_load_path);
}

static fs::path get_dll_path(std::string const& script_name)
{
    std::string version = eval_string(
        "out = "
        "str(bpy.app.version[0]) + '.' +"
        "str(bpy.app.version[1]) + '.' +"
        "str(bpy.app.version[2])"
    );
    return root_path / "lib" / (script_name + "-" + version + SL_EXT);
}

static void load_script(std::string const& script_name)
{
    fs::path dll_path = get_dll_path(script_name);
    if(!fs::exists(dll_path))
    {
        std::cout << "Error: Could not find library file " << dll_path << "\n";
        return;
    }

    auto old = libraries.find(script_name);
    if(old != libraries.end())
    {
        unload_script(old->second);
        libraries.erase(old);
    }

    // copy to a separate file so we can rebuild in peace
    fs::file_time_type last_write = fs::last_write_time(dll_path);
    fs::path dll_load_path = dll_path.string() + ".load";

    try
    {
        // unix: todo: add lock file writing here
        // windows: throws exception if compiler is writing the file, just ignore that and try again
        fs::copy(dll_path,dll_load_path,fs::copy_options::overwrite_existing);
    }
    catch(...) // todo: specify error
    {
        return;
    }

    sl_ptr_ct dll = SL_LOAD(dll_load_path.string().c_str());

    if (dll == nullptr)
    {
        std::cout << "Error: Could not open library file for script " << script_name << "\n";
        return;
    }

    libraries[script_name] = {last_write,dll,script_name,dll_load_path};

    script_register_ct script_register = (script_register_ct) SL_FN(dll, "_script_register");
    if(!script_register)
    {
        std::cout << "Error: Could not find registry function for script " << script_name << "\n";
        return;
    }

    script_register(script_name.c_str(), &functions);
}

void __init_pointers_store(shared_functions* functions);
void setup_cxx(
    char* path,
    cy_exec_ct cy_exec,
    cy_eval_ptr_ct cy_eval_ptr,
    cy_eval_int_ct cy_eval_int,
    cy_eval_float_ct cy_eval_float,
    cy_eval_string_ct cy_eval_string,
    cy_unregister_script_ct cy_unregister_script
)
{
    root_path = fs::path(path);
    functions = {cy_exec,cy_eval_ptr,cy_eval_int,cy_eval_float,cy_eval_string};
    _cy_unregister_script = cy_unregister_script;
    __init_pointers_store(&functions);
}

static std::vector<fs::path> get_scripts()
{
    std::vector<fs::path> paths = { root_path / "bxx" / "tests" };
    for (auto const& dir_entry : fs::directory_iterator{ root_path / "scripts" })
    {
        paths.push_back(dir_entry.path());
    }
    return paths;
}

void auto_reload_cxx()
{
    for (auto const& dir_entry : get_scripts())
    {
        std::string script_name = dir_entry.filename().string();
        auto old = libraries.find(script_name);
        if (old == libraries.end() || old->second.m_ctime != fs::last_write_time(get_dll_path(script_name)))
        {
            load_script(script_name);
        }
    }
}

void run_tests(char* include, char* exclude)
{
    std::cout << "== Running Tests == \n";
    for (auto const& [name,library] : libraries)
    {
        __register_tests_ct __register_tests = (__register_tests_ct)SL_FN(library.m_library, "__register__tests");
        if (__register_tests)
        {
            bxx::test_collection* col = __register_tests();
            if (col)
            {
                for (int i = 0; i < col->m_count; ++i)
                {
                    std::cout << "Test:" << name << "_" << col->m_entries[i].m_name << "\n";
                    col->m_entries[i].test_ptr();
                }
            }
        }
    }
}

void register_cxx()
{
    for (auto const& dir_entry : get_scripts())
    {
        load_script(dir_entry.filename().string());
    }
}

void unregister_cxx()
{
    for (auto& [_, script] : libraries)
    {
        unload_script(script);
    }
    libraries.clear();
}

typedef void (*fire_operator_ct)(char* op, char* json);

extern "C" {
    void _fire_operator(char* script, char* op, char* json)
    {
        auto itr = libraries.find(script);
        if (itr == libraries.end())
        {
            std::cout << "Error: Attempted to call operator " << op << " in non-existing script " << script << "\n";
            return;
        }
        fire_operator_ct cb = (fire_operator_ct)SL_FN(itr->second.m_library, "_fire_operator");
        if (cb)
        {
            cb(op, json);
        }
        else
        {
            std::cout << "Error: Could not find declaration for _fire_operator script \"" << script << "\"\n";
        }
    }
}
