#include "tests.hpp"

#include <bxx/blender_types/armature.hpp>
#include <bxx/blender_types/object.hpp>
#include <bxx/blender_types/context.hpp>
#include <bxx/blender_types/view_layer.hpp>
#include "exec.hpp"
#include "fmt/core.h"

BXX_TEST(create_armature)
{
    BXX_ASSERT_EQUAL(bxx::armature::create("test-armature").get_name(),"test-armature");
}

BXX_TEST(create_armature_object)
{
    bxx::armature armature = bxx::armature::create("test-armature");
    bxx::object object = bxx::object::create("test-object", armature);
}

BXX_TEST(add_armature_bone)
{
    // note: cannot run in unit tests because of context stuff, should be fixable
    //bxx::armature arma = bxx::armature::create("test-armature");
    //bxx::object obj = bxx::object::create("test-object", arma);
    //bxx::context::link_object(obj);
    //bxx::context::set_active_object(obj);
    //bxx::context::set_mode(bxx::editor_mode::EDIT);
    //bxx::bone bone = arma.add_bone("test-bone");
    //bone.set_roll(3.15);
    //bxx::context::set_mode(bxx::editor_mode::OBJECT);
    //bxx::context::update();
}