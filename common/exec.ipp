#include <common/exec.hpp>
#include <common/shared_functions.hpp>

namespace bxx
{
    template <typename ...Args>
    void exec(fmt::format_string<Args...> str, Args... args)
    {
        exec({ fmt::format(str, std::forward<Args>(args)...).c_str() });
    }

    template <typename ...Args>
    int eval_int(fmt::format_string<Args...> str, Args... args)
    {
        return eval_int({ fmt::format(str, std::forward<Args>(args)...).c_str() });
    }

    template <typename ...Args>
    float eval_float(fmt::format_string<Args...> str, Args... args)
    {
        return eval_float({ fmt::format(str, std::forward<Args>(args)...).c_str() });
    }

    template <typename ...Args>
    std::string eval_string(fmt::format_string<Args...> str, Args... args)
    {
        return eval_string({ fmt::format(str, std::forward<Args>(args)...).c_str() });
    }

    template <typename T, typename ...Args>
    T* eval_ptr(fmt::format_string<Args...> str, Args... args)
    {
        return eval_ptr<T>({ fmt::format(str, std::forward<Args>(args)...).c_str() });
    }

    template <typename T>
    T* eval_ptr(std::initializer_list<std::string> lines)
    {
        std::string text = join_strings(lines);
        PyObject* obj = get_pointers()->cy_eval(
            const_cast<char*>(join_strings(lines).c_str())
        );
        BXX_SCRIPT_ASSERT(obj, python_exec_error, "Failed to evaluate python pointer (script error): {}", text.c_str());
        BXX_SCRIPT_ASSERT(PyLong_Check(obj), python_type_error, "Failed to evaluate python pointer (incorrect type): {}", text.c_str());
        return reinterpret_cast<T*>(PyLong_AsUnsignedLongLong(obj));
    }
}
