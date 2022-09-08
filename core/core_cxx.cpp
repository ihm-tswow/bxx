// relative imports because cmake can't add include directories for custom targets

#include <common/shared_functions.hpp>
#include <common/exec.hpp>
#include <common/tests.hpp>
#include <common/util.hpp>
#include <common/addon.hpp>
#include <core/core_defines.hpp>

#include <iostream>
#include <filesystem>
#include <map>
#include <vector>

struct library_handle
{
    library_handle(fs::file_time_type ctime, size_t index, std::string const& name, fs::path load_path)
        : m_ctime(ctime)
        , m_index(index)
        , m_name(name)
        , m_load_path(load_path)
    {}

    library_handle() = default;

    fs::file_time_type m_ctime;
    size_t m_index;
    std::string m_name;
    fs::path m_load_path;
    sl_ptr_ct m_library = nullptr;
    lib_fire_event_ct m_fire_event = nullptr;

    void unload()
    {
        m_library = nullptr;
        m_fire_event = nullptr;
    }
};

void init_pointers_store(shared_functions* pointers);
static shared_functions functions;
static fs::path root_path;
static std::vector<library_handle> libraries;

static auto find_library(std::string const& name)
{
    return std::find_if(libraries.begin(), libraries.end(), [&](library_handle const& lib) { return lib.m_name == name; });
}

static std::string get_library_extension()
{
    // todo: this is wrong on 2.79b
    static std::string blender_version = eval_string(
        "out = "
        "str(bpy.app.version[0]) + '.' +"
        "str(bpy.app.version[1]) + '.' +"
        "str(bpy.app.version[2])"
    );
    static std::string extension = "-" + blender_version + SL_EXT;
    return extension;
}

// Unloads a single script library
static void unload_script(library_handle & handle)
{
    script_unregister_ct script_unregister = (script_unregister_ct)SL_FN(handle.m_library, "lib_script_unregister");
    if (script_unregister)
    {
        script_unregister();
    }
    exec("unregister_script({})", handle.m_index);
    SL_CLOSE(handle.m_library);
    handle.unload();
    fs::remove(handle.m_load_path);
}

// Loads a single script library
static void load_script(fs::path const& dll_path)
{
    std::string const& script_name = dll_path.filename().string();

    // copy to a separate file so we can rebuild in peace
    fs::file_time_type last_write = fs::last_write_time(dll_path);
    fs::path dll_load_path = dll_path.string() + ".load";

    library_handle* handle;
    auto old = find_library(script_name);
    if(old != libraries.end())
    {
        unload_script(*old);
        old->m_ctime = last_write;
        handle = &*old;
    }
    else
    {
        size_t index = libraries.size();
        handle = &libraries.emplace_back(last_write, index, script_name, dll_load_path);
    }

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

    handle->m_library = SL_LOAD(dll_load_path.string().c_str());

    if (!handle->m_library)
    {
        std::cout << "Error: Could not open library file for script " << script_name << "\n";
        return;
    }

    script_register_ct script_register = (script_register_ct) SL_FN(handle->m_library, "lib_script_register");
    handle->m_fire_event = (lib_fire_event_ct) SL_FN(handle->m_library, "lib_fire_event");

    if(!script_register)
    {
        std::cout << "Error: Could not find registry function for script " << script_name << "\n";
        return;
    }

    std::cout << "Loading script " << script_name << "\n";
    script_register(script_name.c_str(), handle->m_index, &functions);
}

// Returns all script library paths
static std::vector<fs::path> get_scripts()
{
    std::vector<fs::path> paths;
    for (auto const& dir_entry : fs::directory_iterator{ root_path / "lib" })
    {
        if (bxx::ends_with(dir_entry.path().string(), get_library_extension()))
        {
            paths.push_back(dir_entry.path());
        }
    }
    return paths;
}

//
// Cython API
//

extern "C" {
    void setup_cxx(
        char* path,
        cy_exec_ct exec,
        cy_eval_ct eval,
        cy_create_float_buffer_ct create_float_buffer,
        cy_apply_image_buffer_ct apply_image_buffer,
        cy_delete_image_buffer_ct delete_image_buffer
    )
    {
        root_path = fs::path(path);
        functions = {
            exec,
            eval,
            create_float_buffer,
            apply_image_buffer,
            delete_image_buffer
        };
        init_pointers_store(&functions);
    }

    void auto_reload_cxx()
    {
        for (auto const& dir_entry : get_scripts())
        {
            std::string script_name = dir_entry.filename().string();
            auto old = find_library(script_name);
            if (old == libraries.end() || old->m_ctime != fs::last_write_time(dir_entry))
            {
                load_script(dir_entry);
            }
        }
    }

    void register_cxx()
    {
        for (auto const& dir_entry : get_scripts())
        {
            load_script(dir_entry);
        }
    }

    void unregister_cxx()
    {
        for (library_handle & library : libraries)
        {
            unload_script(library);
        }
    }

    PyObject* core_fire_event(size_t script, size_t evt, PyObject* obj)
    {
        return libraries[script].m_fire_event(evt, obj);
    }

    int run_tests(char* include, char* exclude)
    {
        exec("bpy.ops.wm.read_homefile(use_empty=True)");
        std::cout
            << "\n\n"
            << "Running Tests: "
            << bxx::color_code::YELLOW
            << bxx::get_addon_name()
            << bxx::color_code::DEFAULT
            << "\n"
            ;

        int total_tests = 0;
        int successful_tests = 0;
        int failed_tests = 0;
        int skipped_tests = 0;
        bool has_include = strlen(include) > 0;
        bool has_exclude = strlen(exclude) > 0;

        if (has_include)
        {
            std::cout << bxx::color_code::GRAY << "include: " << include << bxx::color_code::DEFAULT << "\n";
        }

        if (has_exclude)
        {
            std::cout << bxx::color_code::GRAY << "exclude: " << include << bxx::color_code::DEFAULT << "\n";
        }

        for (library_handle & library : libraries)
        {
            register_tests_ct lib_register_tests_ptr = (register_tests_ct)SL_FN(library.m_library, "lib_register_tests");
            if (lib_register_tests_ptr)
            {
                bxx::test_collection* col = lib_register_tests_ptr();
                if (col)
                {
                    for (int i = 0; i < col->m_count; ++i)
                    {
                        total_tests++;
                        std::string full_name =
                            library.m_name.substr(0, library.m_name.size() - int(get_library_extension().size()))
                            + "." + col->m_entries[i].m_name;

                        if (
                            (has_include && !bxx::match(include, full_name)) ||
                            (has_exclude && bxx::match(exclude, full_name))
                            ) {
                            skipped_tests++;
                            continue;
                        }

                        try
                        {
                            col->m_entries[i].test_ptr();
                            exec("bpy.ops.wm.read_homefile(use_empty=True)");
                            successful_tests++;
                        }
                        catch (bxx::test_exception const& except) {
                            failed_tests++;
                        }
                    }
                }
            }
        }

        if (total_tests == 0)
        {
            std::cout << "No tests run\n";
            return 1;
        }

        int grays = 80 * float(skipped_tests) / float(total_tests);
        int greens = 80 * float(successful_tests) / float(total_tests);
        int reds = 80 * float(failed_tests) / float(total_tests);

        std::cout << "\n";
        std::cout << bxx::color_code::RED;
        for (size_t i = 0; i < reds; ++i) std::cout << "=";
        std::cout << bxx::color_code::GREEN;
        for (size_t i = 0; i < greens; ++i) std::cout << "=";
        std::cout << bxx::color_code::GRAY;
        for (size_t i = 0; i < grays; ++i) std::cout << "=";
        std::cout << bxx::color_code::DEFAULT << "\n";

        if (skipped_tests == 0 && failed_tests == 0)
        {
            std::cout
                << bxx::color_code::GREEN << "All tests passed "
                << bxx::color_code::DEFAULT << "(" << total_tests << " tests)\n";
            return 0;
        }

        std::cout << bxx::color_code::DEFAULT << "test cases: " << total_tests;

        if (successful_tests) std::cout << bxx::color_code::GRAY << " | " << bxx::color_code::GREEN << "successful: " << successful_tests;
        if (failed_tests)     std::cout << bxx::color_code::GRAY << " | " << bxx::color_code::RED << "failed: " << failed_tests;
        if (skipped_tests)    std::cout << bxx::color_code::GRAY << " | " << bxx::color_code::GRAY << "skipped: " << skipped_tests;
        std::cout << bxx::color_code::DEFAULT << "\n";
        return failed_tests > 0;
    }
}
