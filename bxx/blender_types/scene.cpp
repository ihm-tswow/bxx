#include <bxx/blender_types/scene.hpp>
#include <bxx/blender_types/view_layer.hpp>
#include <common/exec.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_scene_types.h>
#pragma warning(pop)

#include <fmt/core.h>

namespace bxx
{
    view_layers scene::view_layers()
    {
        return getattr<bxx::view_layers>("view_layers");
    }
}
