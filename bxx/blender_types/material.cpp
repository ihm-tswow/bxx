#include <bxx/blender_types/material.hpp>
#include <common/exec.hpp>

#pragma warning(push)
#pragma warning(disable : 4200)
#include <makesdna/DNA_material_types.h>
#include <makesdna/DNA_Node_types.h>
#pragma warning(pop)

#include <fmt/core.h>
#include <magic_enum.hpp>

namespace bxx
{
    std::string node_type_to_string(material_node_type type)
    {
        return std::string(magic_enum::enum_name<material_node_type>(type));
    }

    material_node::material_node(material const& parent, python_object const& obj)
        : blender_py_struct<bl_material_node>(obj)
        , m_parent(parent)
    {}

    material material_node::get_parent()
    {
        return m_parent;
    }

    std::string material_node::get_name() const
    {
        return get_raw_struct()->name;
    }

    material_node material::add_node(std::string const& type, mathutils::vec2 const& location)
    {
        set_use_nodes(true);
        material_node node = material_node(*this,
            getattr("node_tree").getattr("nodes").call("new",type)
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
            getattr("node_tree").getattr("nodes").get_item(name)
        );
    }

    void material_node::_set_default_input_code(int index, std::string const& code)
    {
        // todo: where are the arguments?
        //exec("{}.inputs[{}].default_value = {code}");
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
        getattr("node_tree").getattr("links").call("new"
            , output_node.getattr("outputs").get_item(output_socket)
            , input_node.getattr("inputs").get_item(input_socket)
        );
    }
}
