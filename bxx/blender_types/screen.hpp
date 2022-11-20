#pragma once

#include <bxx/objects/python_object.hpp>
#include <bxx/blender_types/iterables.hpp>
#include <bxx/blender_types/object.hpp>

#include <optional>

namespace mathutils
{
    struct vec3;
}

namespace bxx
{
    enum class screen_space_type
    {
        VIEW_3D,
        IMAGE_EDITOR,
        NODE_EDITOR,
        SEQUENCE_EDITOR,
        CLIP_EDITOR,
        DOPESHEET_EDITOR,
        GRAPH_EDITOR,
        NLA_EDITOR,
        TEXT_EDITOR,
        CONSOLE,
        INFO,
        TOPBAR,
        STATUSBAR,
        OUTLINER,
        PROPERTIES,
        FILE_BROWSER,
        SPREADSHEET,
        PREFERENCES,
    };

    class region_view_3d : public python_object
    {
    public:
        using python_object::python_object;
        PYFIELD_DECL(mathutils::vec3, view_location);
        mathutils::vec3 get_view_source_location();
        PYFIELD_DECL(mathutils::quaternion, view_rotation);
        PYFIELD_DECL(float, view_distance);
    };

    class screen_space : public python_object
    {
    public:
        using python_object::python_object;
        PYFIELD_STRINGENUM_DECL(screen_space_type, type)
    };

    class screen_space_view_3d : public screen_space
    {
    public:
        screen_space_view_3d(screen_space const& space);
        PYFIELD_DECL(bxx::object, camera);
        PYFIELD_DECL(region_view_3d, region_3d);
    };

    class screen_area : public python_object
    {
    public:
        using python_object::python_object;
        blender_py_iterable<screen_space> get_spaces();
    };

    class screen : public python_object
    {
    public:
        using python_object::python_object;
        blender_py_iterable<screen_area> get_areas();
    };
}

#include <bxx/blender_types/screen.ipp>