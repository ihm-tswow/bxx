#include <bxx/blender_types/data.hpp>

#include <bxx/blender_types/action.hpp>
#include <bxx/blender_types/collection.hpp>
#include <bxx/blender_types/image.hpp>
#include <bxx/blender_types/object.hpp>
#include <bxx/blender_types/armature.hpp>
#include <bxx/blender_types/mesh.hpp>
#include <bxx/blender_types/material.hpp>
#include <bxx/blender_types/scene.hpp>

#define DATA_IMPL(type)\
    type type##_data::create(std::string const& name)\
    {\
        return call<type>("new",kwarg("name",name));\
    }\
    \
    type type##_data::find(std::string const& name)\
    {\
        return get_item<type>(name);\
    }\

namespace bxx
{
    action_data data::actions() { return getattr<action_data>("actions"); }
    object_data data::objects() { return getattr<object_data>("objects"); }
    armature_data data::armatures() { return getattr<armature_data>("armatures"); }
    mesh_data data::meshes() { return getattr<mesh_data>("meshes"); }
    material_data data::materials() { return getattr<material_data>("materials"); }
    scene_data data::scenes() { return getattr<scene_data>("scenes"); }
    image_data data::images() { return getattr<image_data>("images"); }
    collection_data data::collections() { return getattr<collection_data>("collections"); }

    data get_data()
    {
        return data(eval_pyobject("out = bpy.data"));
    }

    DATA_IMPL(action)
    DATA_IMPL(armature)
    DATA_IMPL(mesh)
    DATA_IMPL(material)
    DATA_IMPL(scene)
    DATA_IMPL(collection)

    image image_data::create(std::string const& name, int width, int height)
    {
        return image(call("new", name, width, height), width, height);
    }

    image image_data::find(std::string const& name, int width, int height)
    {
        return image(get_item(name), width, height);
    }

    object object_data::create(std::string const& name, mesh const& obj)
    {
        return call<object>("new", name, obj);
    }

    object object_data::create(std::string const& name, armature const& obj)
    {
        return call<object>("new", name, obj);
    }

    object object_data::find(std::string const& name)
    {
        return get_item(name);
    }
}