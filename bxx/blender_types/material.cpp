#include "material.hpp"
#include "exec.hpp"

#include "fmt/core.h"

#include "magic_enum.hpp"

namespace bxx
{
    std::string node_type_to_string(material_node_type type)
    {
        return std::string(magic_enum::enum_name<material_node_type>(type));
    }

    material_node::material_node(material const& parent, bl_material_node* node)
        : blender_py_struct<bl_material_node>(node)
        , m_parent(parent)
    {}

    python_object material_node::get_pyobject()
    {
        return eval_pyobject(fmt::format("out = {}.node_tree.nodes['{}']", get_parent().get_name_full(), get_name()));
    }

    material material_node::get_parent()
    {
        return m_parent;
    }

    std::string material_node::get_name() const
    {
        return get_raw_struct()->name;
    }

    material material::create(std::string const& name)
    {
        return eval_ptr<bl_material>({
            fmt::format("out = bpy.data.materials.new(name='{}').as_pointer()", name)
        });
    }

    std::string material::get_type_path() const
    {
        return "bpy.data.materials";
    }

    material_node material::add_node(std::string const& type, mathutils::vec2 const& location)
    {
        set_use_nodes(true);
        material_node node = material_node(*this,
            eval_ptr<bl_material_node>({
                fmt::format("out = {}.node_tree.nodes.new('{}').as_pointer()", get_name_full(), type)
            })
        );
        node.set_location(location);
        return node;
    }

    material_node material::add_node(material_node_type type, mathutils::vec2 const& location)
    {
        return add_node(node_type_to_string(type), location);
    }

    material_node material::get_node(std::string const& name)
    {
        return material_node(*this,
            eval_ptr<bl_material_node>({
                fmt::format("out = {}.node_tree.nodes['{}'].as_pointer()", get_name_full(), name)
            })
        );
    }

    std::string material_node::get_name_full() const
    {
        return fmt::format("{}.node_tree.nodes['{}']", m_parent.get_name_full(), get_name());
    }

    void material_node::_set_default_input_code(int index, std::string const& code)
    {
        exec({
            "{}.inputs[{}].default_value = {code}"
        });
    }

    void material_node::set_default_input(int index, std::string const& value)
    {
        _set_default_input_code(index, fmt::format("\"{}\"", value));
    }

    void material_node::set_default_input(int index, int value)
    {
        _set_default_input_code(index, fmt::format("{}", value));
    }

    void material_node::set_default_input(int index, float v1)
    {
        _set_default_input_code(index, fmt::format("{}", v1));
    }

    void material_node::set_default_input(int index, bool value)
    {
        _set_default_input_code(index, fmt::format("{}", value));
    }

    void material_node::set_default_input(int index, std::initializer_list<float> const& list)
    {
        std::string code = "(";
        for (float f : list) code += std::to_string(f);
        code += ")";
        _set_default_input_code(index, code);
    }

    void material::connect(material_node const& output_node, int output_socket, material_node const& input_node, int input_socket)
    {
        exec({
            fmt::format("{}.node_tree.links.new({}.outputs[{}], {}.inputs[{}])"
                , get_name_full()
                , output_node.get_name_full()
                , output_socket
                , input_node.get_name_full()
                , input_socket
            )
            });
    }
}
