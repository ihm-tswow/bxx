#include <bxx/blender_types/screen.hpp>

namespace bxx
{
    PYFIELD_IMPL(region_view_3d, mathutils::vec3, view_location);
    PYFIELD_IMPL(region_view_3d, mathutils::quaternion, view_rotation);
    PYFIELD_IMPL(region_view_3d, float, view_distance);

    PYFIELD_IMPL(screen_space_view_3d,bxx::object, camera);
    PYFIELD_IMPL(screen_space_view_3d, region_view_3d, region_3d);

    PYFIELD_STRINGENUM_IMPL(screen_space,screen_space_type, type)

    mathutils::vec3 region_view_3d::get_view_source_location()
    {
        return getattr("view_matrix").call("inverted").getattr<mathutils::vec3>("translation");
    }

    screen_space_view_3d::screen_space_view_3d(screen_space const& space)
        : screen_space(space.get_type() == screen_space_type::VIEW_3D ? space : python_object())
    {
    }

    blender_py_iterable<screen_area> screen::get_areas()
    {
        return getattr<blender_py_iterable<screen_area>>("areas");
    }

    blender_py_iterable<screen_space> screen_area::get_spaces()
    {
        return getattr<blender_py_iterable<screen_space>>("spaces");
    }
}