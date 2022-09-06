#include <bxx/blender_types/collection.hpp>
#include <bxx/blender_types/object.hpp>
#include <common/exec.hpp>

#include <fmt/core.h>

namespace bxx
{
    collection collection::create(std::string const& name)
    {
        return eval_pyobject("out = bpy.data.collections.new(name='{}')", name);
    }

    void collection::link_object(bxx::object const& obj)
    {
        getattr("objects").call("link", obj);
    }

    collection_objects collection::objects()
    {
        return getattr<collection_objects>("objects");
    }

    collection_children collection::children()
    {
        return getattr<collection_children>("children");
    }

    void collection_objects::add(object obj)
    {
        call("link", obj);
    }

    void collection_children::add(collection child)
    {
        call("link", child);
    }
}
