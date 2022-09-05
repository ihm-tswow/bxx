#include <bxx/blender_types/object.hpp>
#include <bxx/blender_types/mesh.hpp>
#include <bxx/blender_types/constraint.hpp>
#include <common/tests.hpp>
#include <iostream>

using namespace bxx;

BXX_TEST(create_basic_constraint)
{
    bxx::mesh m = bxx::mesh::create("constraint_mesh");
    bxx::object o = bxx::object::create("constraint_object",m);
    o.add_constraint<action_constraint>("my_action_constraint");
    BXX_ASSERT_EQUAL(o.constraints_len(), 1);
    BXX_ASSERT_NOT_EQUAL(o.get_constraint<action_constraint>(0).get_raw_struct(), nullptr);
}
