#include <common/tests.hpp>

#include <bxx/blender_types/data.hpp>
#include <bxx/blender_types/armature.hpp>
#include <bxx/blender_types/object.hpp>
#include <bxx/blender_types/context.hpp>
#include <bxx/blender_types/view_layer.hpp>
#include <common/exec.hpp>
#include <fmt/core.h>

using namespace bxx;

BXX_TEST(create_armature)
{
    BXX_ASSERT_EQUAL(get_data().armatures().create("test-armature").get_name(), "test-armature");
}

BXX_TEST(create_armature_object)
{
    armature armature = get_data().armatures().create("test-armature");
    object object = get_data().objects().create("test-object", armature);
}

BXX_TEST(add_armature_bone)
{
    // note: cannot run in unit tests because of context stuff, should be fixable
    //armature arma = armature::create("test-armature");
    //object obj = object::create("test-object", arma);
    //context::link_object(obj);
    //context::set_active_object(obj);
    //context::set_mode(editor_mode::EDIT);
    //bone bone = arma.add_bone("test-bone");
    //bone.set_roll(3.15);
    //context::set_mode(editor_mode::OBJECT);
    //context::update();
}