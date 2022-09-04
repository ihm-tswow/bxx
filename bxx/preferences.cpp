#include <bxx/preferences.hpp>
#include <bxx/objects/python_object.hpp>
#include <common/exec.hpp>
#include <common/addon.hpp>

#include <fmt/core.h>

namespace bxx
{
    python_object get_addon_preferences()
    {
        return eval_pyobject("out = bpy.context.preferences.addons[\"{}\"].preferences", get_addon_name());
    }
}