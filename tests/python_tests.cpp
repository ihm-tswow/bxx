#include <bxx/objects/python_object.hpp>
#include <bxx/objects/python_list.hpp>
#include <bxx/objects/python_tuple.hpp>
#include <bxx/objects/python_dict.hpp>
#include <bxx/objects/blender_struct.hpp>
#include <common/tests.hpp>
#include <common/exec.hpp>

using namespace bxx;

BXX_TEST(python_object_empty_object) {
    BXX_ASSERT_EQUAL(python_object().get_pyobject(), Py_None);
}

BXX_TEST(python_list_initializes_empty) {
    BXX_ASSERT_NOT_EQUAL(python_list().get_pyobject(), Py_None);
    BXX_ASSERT_NOT_EQUAL(python_list().get_pyobject(), nullptr);
    BXX_ASSERT_EQUAL(python_list().ref_count(), 1);
}

BXX_TEST(python_dict_initializes_empty) {
    BXX_ASSERT_NOT_EQUAL(python_dict().get_pyobject(), Py_None);
    BXX_ASSERT_NOT_EQUAL(python_dict().get_pyobject(), nullptr);
    BXX_ASSERT_EQUAL(python_dict().ref_count(), 1);
}

BXX_TEST(python_object_releases_refs) {
    PyObject* raw;
    {
        python_list list;
        raw = list.get_pyobject();
    }
    BXX_ASSERT_EQUAL(Py_REFCNT(raw), 0);
}

BXX_TEST(python_object_copies_object_refs) {
    python_list l1;
    python_list l2 = l1;
    BXX_ASSERT_EQUAL(l1.ref_count(), 2);
}

BXX_TEST(python_list_append) {
    python_list l1;
    python_list l2;
    l1.append(l2);
    BXX_ASSERT_EQUAL(l1.len(),1);
    l1.append(10);
    BXX_ASSERT_EQUAL(l1.len(),2);
    BXX_ASSERT_EQUAL(l2.ref_count(), 2);
    BXX_ASSERT_EQUAL(l1.get<python_list>(0).get_pyobject(), l2.get_pyobject());
    BXX_ASSERT_EQUAL(l1.get<int>(1), 10);
}

BXX_TEST(python_list_set) {
    python_list l1(2);
    python_list l2;
    l1.set(0, l2);
    l1.set(1, 10);
    BXX_ASSERT_EQUAL(l2.ref_count(), 2);
    BXX_ASSERT_EQUAL(l1.get<python_list>(0).get_pyobject(), l2.get_pyobject());
    BXX_ASSERT_EQUAL(l1.get<int>(1), 10);
}

BXX_TEST(python_list_from_vector) {
    std::vector<int> my_vec = { 25,1007688 };
    python_list li(my_vec);
    BXX_ASSERT_EQUAL(li.len(), my_vec.size());
    BXX_ASSERT_EQUAL(li.get<int>(0), my_vec[0]);
    BXX_ASSERT_EQUAL(li.get<int>(1), my_vec[1]);
}

BXX_TEST(python_tuple_set) {
    python_tuple tup(2);
    python_list list;
    tup.set(0, list);
    tup.set(1, 10);
    BXX_ASSERT_EQUAL(list.ref_count(), 2);
    BXX_ASSERT_EQUAL(tup.get<python_list>(0).get_pyobject(), list.get_pyobject());
    BXX_ASSERT_EQUAL(tup.get<int>(1), 10);
}

BXX_TEST(python_dict_set) {
    python_dict dict;
    python_list list;
    dict.set("k1", list);
    dict.set("k2", 10);
    BXX_ASSERT_EQUAL(list.ref_count(), 2);
    BXX_ASSERT_EQUAL(dict.get<python_list>("k1").get_pyobject(), list.get_pyobject());
    BXX_ASSERT_EQUAL(dict.get<int>("k2"), 10);
}

BXX_TEST(python_dict_del) {
    python_dict dict;
    python_list list;
    dict.set("k1", list);
    dict.del("k1");
    BXX_ASSERT_EQUAL(list.ref_count(), 1);
}

python_object create_python_object()
{
    return eval_pyobject(
        "class TestClass:\n"
        "    def __init__(self):\n"
        "        pass\n"
        "out = TestClass()\n"
    );
}

BXX_TEST(python_object_create) {
    python_object obj = create_python_object();
    BXX_ASSERT_NOT_EQUAL(obj.get_pyobject(), nullptr);
    BXX_ASSERT_NOT_EQUAL(obj.get_pyobject(), Py_None);
}

BXX_TEST(python_object_setattr) {
    python_object obj = create_python_object();
    python_list list;
    obj.setattr("list", list);
    BXX_ASSERT_EQUAL(list.ref_count(), 2);
    BXX_ASSERT_EQUAL(obj.getattr<python_list>("list").get_pyobject(), list.get_pyobject());
}

BXX_TEST(python_object_delattr) {
    python_object obj = create_python_object();
    python_list list;
    obj.setattr("list", list);
    obj.delattr("list");
    BXX_ASSERT_EQUAL(list.ref_count(), 1);
}

BXX_TEST(python_object_call_no_args) {
    python_object obj = eval_pyobject(
        "class TestClass:\n"
        "    def __init__(self):\n"
        "        pass\n"
        "    def call(self):\n"
        "        return 10\n"
        "out = TestClass()"
    );
    BXX_ASSERT_EQUAL(obj.call<int>("call"),10);
}

BXX_TEST(python_object_call_simple_args) {
    python_object obj = eval_pyobject(
        "class TestClass:\n"
        "    def __init__(self):\n"
        "        pass\n"
        "    def call(self,a):\n"
        "        return a\n"
        "out = TestClass()\n"
    );
    BXX_ASSERT_EQUAL(obj.call<int>("call",25),25);
}

BXX_TEST(python_object_call_varargs) {
    python_object obj = eval_pyobject(
        "class TestClass:\n"
        "    def __init__(self):\n"
        "        pass\n"
        "    def call(self,*args):\n"
        "        return args[0] + args[1]\n"
        "out = TestClass()"
    );
    BXX_ASSERT_EQUAL(obj.call<int>("call",25,10),35);
}

BXX_TEST(python_object_call_kwarg) {
    python_object obj = eval_pyobject(
        "class TestClass:\n"
        "    def __init__(self):\n"
        "        pass\n"
        "    def call(self,a,b):\n"
        "        return a\n"
        "out = TestClass()"
    );
    BXX_ASSERT_EQUAL(obj.call<int>("call",kwarg{"b",25}, kwarg{"a",30}), 30);
}

BXX_TEST(python_object_complex_arg) {
    python_list list;
    list.append(10);
    python_object obj = eval_pyobject(
        "class TestClass:\n"
        "    def __init__(self):\n"
        "        pass\n"
        "    def call(self,a):\n"
        "        return a\n"
        "out = TestClass()"
    );
    BXX_ASSERT_EQUAL(obj.call<python_list>("call",list).get<int>(0), 10);
    BXX_ASSERT_EQUAL(list.ref_count(), 1);
}

// I just needed a sanity check for if this actually works
BXX_TEST(python_object_kwarg_primitive_refcount) {
    details::python_tempref r(details::cxx2py(std::uint32_t(10), false));
    size_t cur_refcount = r.m_pyobj->ob_refcnt;
    python_object obj = eval_pyobject(
        "class TestClass:\n"
        "    def __init__(self):\n"
        "        pass\n"
        "    def call(self,a):\n"
        "        return a\n"
        "out = TestClass()"
        );
    obj.call<python_object>("call", kwarg("a",r.m_pyobj));
    BXX_ASSERT_EQUAL(cur_refcount, r.m_pyobj->ob_refcnt);
}

BXX_TEST(python_object_complex_kwarg) {
    python_list list;
    list.append(10);
    python_object obj = eval_pyobject(
        "class TestClass:\n"
        "    def __init__(self):\n"
        "        pass\n"
        "    def call(self,a,b):\n"
        "        return a\n"
        "out = TestClass()"
    );
    BXX_ASSERT_EQUAL(obj.call<python_list>("call",kwarg("b",5),kwarg<python_list>("a",list)).get<int>(0), 10);
    BXX_ASSERT_EQUAL(list.ref_count(), 1);
}
