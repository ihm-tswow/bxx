#include <common/addon.hpp>
#include <common/exec.hpp>

namespace bxx
{
    std::string get_addon_name()
    {
        return eval_string("out = get_addon_name()");
    }

    fs::path get_addon_path()
    {
        return eval_string("out = get_addon_path()");
    }
}

