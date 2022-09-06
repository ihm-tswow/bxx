#include <bxx/blender_types/data.hpp>
#include <bxx/blender_types/object.hpp>
#include <bxx/blender_types/mesh.hpp>
#include <bxx/blender_types/constraint.hpp>
#include <bxx/blender_types/armature.hpp>
#include <bxx/blender_types/action.hpp>
#include <bxx/blender_types/context.hpp>
#include <common/tests.hpp>
#include <iostream>

using namespace bxx;

BXX_TEST(create_basic_constraint)
{
    mesh m = get_data().meshes().create("constraint_mesh");
    object o = get_data().objects().create("constraint_object",m);
    o.constraints().add<action_constraint>("my_action_constraint");
    BXX_ASSERT_EQUAL(o.constraints().len(), 1);
    BXX_ASSERT_NOT_EQUAL(o.constraints().get<action_constraint>(0).get_raw_struct(), nullptr);
}

BXX_TEST(constraint_action_reference)
{
    // todo: this seems to be some kind of blender bug, i cannot get this to work even in python

    //action a = action::create("my_action");
    //mesh m = mesh::create("constraint_mesh");
    //object o = object::create("constraint_object",m);
    //action_constraint con = o.add_constraint<action_constraint>("armature_constraint");
    //con.set_action(a);
    //BXX_ASSERT_EQUAL(con.get_action().get_name(), a.get_name());
}


BXX_TEST(constraint_object_reference_obj)
{
    mesh m1 = get_data().meshes().create("mesh");
    object o1 = get_data().objects().create("obj",m1);
    get_context().link_object(o1);

    mesh m2 = get_data().meshes().create("mesh2");
    object o2 = get_data().objects().create("obj2",m1);

    action_constraint con = o2.constraints().add<action_constraint>("armature_constraint");
    con.set_target(o1);
    BXX_ASSERT_EQUAL(con.get_target().get_raw_struct(), o1.get_raw_struct());
}
