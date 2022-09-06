#include <bxx/blender_types/data.hpp>
#include <bxx/blender_types/material.hpp>
#include <common/tests.hpp>

using namespace bxx;

BXX_TEST(create_material)
{
    BXX_ASSERT_EQUAL(get_data().materials().create("test_material").get_name(),"test_material");
}

BXX_TEST(create_material_nodes)
{
    material mat = get_data().materials().create("test_material");
    mat.add_node(material_node_type::ShaderNodeTexImage);
}

BXX_TEST(connect_material_nodes)
{
    material mat = get_data().materials().create("test_material");
    material_node output = mat.add_node(material_node_type::ShaderNodeTexImage);
    material_node bsdf = mat.get_node("Principled BSDF");
    mat.connect(output, 0, bsdf, 0);
}
