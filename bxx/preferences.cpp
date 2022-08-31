#include "preferences.hpp"
#include "python_object.hpp"
#include "exec.hpp"
#include "addon.hpp"

#include <fmt/core.h>

namespace bxx
{
    python_object get_addon_preferences()
    {
        return eval_pyobject(fmt::format("out = bpy.context.preferences.addons[\"{}\"].preferences", get_addon_name()));
    }
}