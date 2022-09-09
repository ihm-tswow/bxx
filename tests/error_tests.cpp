#include <bxx/objects/python_object.hpp>
#include <common/exec.hpp>
#include <common/tests.hpp>

using namespace bxx;

#define ASSERT_THROWS(expression, error_type) try { expression; BXX_ASSERT(false); } catch(error_type const&) {} catch(...) { BXX_ASSERT(false); }

BXX_TEST(python_object_errors)
{
    python_object obj;
    ASSERT_THROWS(obj.call("key"), python_object_error)
    ASSERT_THROWS(obj.getattr("key"), python_object_error)
    ASSERT_THROWS(obj.get_item("key"), python_object_error)
    ASSERT_THROWS(obj.setattr("key",0), python_object_error)
    ASSERT_THROWS(obj.set_item("key",0), python_object_error)
    ASSERT_THROWS(obj.del_item("key"), python_object_error)
    ASSERT_THROWS(obj.delattr("key"), python_object_error)
}

BXX_TEST(python_key_errors)
{
    python_object obj = eval_pyobject(
        "class H:\n"
        "    pass\n"
        "out = H()"
    );

    ASSERT_THROWS(obj.call("key"), python_key_error)
    ASSERT_THROWS(obj.getattr("key"), python_key_error)
    ASSERT_THROWS(obj.get_item("key"), python_key_error)
    ASSERT_THROWS(obj.delattr("key"), python_key_error)
}

BXX_TEST(python_type_errors)
{
    python_object s("hello");
    python_object i(10);
    ASSERT_THROWS(details::py2cxx<std::uint32_t>(s), python_type_error);
    ASSERT_THROWS(details::py2cxx<std::int32_t>(s), python_type_error);
    ASSERT_THROWS(details::py2cxx<std::uint64_t>(s), python_type_error);
    ASSERT_THROWS(details::py2cxx<std::int64_t>(s), python_type_error);
    ASSERT_THROWS(details::py2cxx<double>(s), python_type_error);
    ASSERT_THROWS(details::py2cxx<float>(s), python_type_error);
    ASSERT_THROWS(details::py2cxx<std::string>(i), python_type_error);
}

BXX_TEST(python_eval_errors)
{
    ASSERT_THROWS(eval_int("raise Exception('')"), python_exec_error);
    ASSERT_THROWS(eval_float("raise Exception('')"), python_exec_error);
    ASSERT_THROWS(eval_ptr<int>("raise Exception('')"), python_exec_error);
    ASSERT_THROWS(eval_pyobject("raise Exception('')"), python_exec_error);
    ASSERT_THROWS(eval_string("raise Exception('')"), python_exec_error);
    ASSERT_THROWS(exec("raise Exception('')"), python_exec_error);

    ASSERT_THROWS(eval_int("out = None"), python_type_error);
    ASSERT_THROWS(eval_float("out = None"), python_type_error);
    ASSERT_THROWS(eval_ptr<int>("out = None"), python_type_error);
    ASSERT_THROWS(eval_string("out = None"), python_type_error);
}
