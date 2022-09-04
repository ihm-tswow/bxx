#include <common/exec.hpp>
#include <common/shared_functions.hpp>

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
    // todo: move cast to c
    return (T*) get_pointers()->cy_eval_ptr(
        const_cast<char*>(join_strings(lines).c_str())
    );
}
