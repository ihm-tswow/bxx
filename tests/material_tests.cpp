#include "tests.hpp"
#include "material.hpp"

BXX_TEST(create_material)
{
    BXX_ASSERT_EQUAL(bxx::material::create("test_material").get_name(),"test_material");
}

BXX_TEST(create_material_nodes)
{
    bxx::material mat = bxx::material::create("test_material");
    mat.add_node(bxx::material_node_type::ShaderNodeTexImage);
}

BXX_TEST(connect_material_nodes)
{
    bxx::material mat = bxx::material::create("test_material");
    bxx::material_node output = mat.add_node(bxx::material_node_type::ShaderNodeTexImage);
    bxx::material_node bsdf = mat.get_node("Principled BSDF");
    mat.connect(output, 0, bsdf, 0);
}
