#include "tests.hpp"

#include "python_object.hpp"
#include "exec.hpp"

using namespace bxx;

BXX_TEST(python_object_empty_object) {
    BXX_ASSERT_EQUAL(python_object().m_obj, Py_None);
}

BXX_TEST(python_list_initializes_empty) {
    BXX_ASSERT_NOT_EQUAL(python_list().m_obj, Py_None);
    BXX_ASSERT_NOT_EQUAL(python_list().m_obj, nullptr);
    BXX_ASSERT_EQUAL(python_list().ref_count(), 1);
}

BXX_TEST(python_dict_initializes_empty) {
    BXX_ASSERT_NOT_EQUAL(python_dict().m_obj, Py_None);
    BXX_ASSERT_NOT_EQUAL(python_dict().m_obj, nullptr);
    BXX_ASSERT_EQUAL(python_dict().ref_count(), 1);
}

BXX_TEST(python_object_releases_refs) {
    PyObject* raw;
    {
        python_list list;
        raw = list.m_obj;
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
    BXX_ASSERT_EQUAL(l1.get<python_list>(0).m_obj, l2.m_obj);
    BXX_ASSERT_EQUAL(l1.get<int>(1), 10);
}

BXX_TEST(python_list_set) {
    python_list l1(2);
    python_list l2;
    l1.set(0, l2);
    l1.set(1, 10);
    BXX_ASSERT_EQUAL(l2.ref_count(), 2);
    BXX_ASSERT_EQUAL(l1.get<python_list>(0).m_obj, l2.m_obj);
    BXX_ASSERT_EQUAL(l1.get<int>(1), 10);
}

BXX_TEST(python_tuple_set) {
    python_tuple tup(2);
    python_list list;
    tup.set(0, list);
    tup.set(1, 10);
    BXX_ASSERT_EQUAL(list.ref_count(), 2);
    BXX_ASSERT_EQUAL(tup.get<python_list>(0).m_obj, list.m_obj);
    BXX_ASSERT_EQUAL(tup.get<int>(1), 10);
}

BXX_TEST(python_dict_set) {
    python_dict dict;
    python_list list;
    dict.set("k1", list);
    dict.set("k2", 10);
    BXX_ASSERT_EQUAL(list.ref_count(), 2);
    BXX_ASSERT_EQUAL(dict.get<python_list>("k1").m_obj, list.m_obj);
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
    return eval_pyobject({
        "class TestClass:",
        "    def __init__(self):",
        "        pass",
        "out = TestClass()"
    });
}

BXX_TEST(python_object_create) {
    python_object obj = create_python_object();
    BXX_ASSERT_NOT_EQUAL(obj.m_obj, nullptr);
    BXX_ASSERT_NOT_EQUAL(obj.m_obj, Py_None);
}

BXX_TEST(python_object_setattr) {
    python_object obj = create_python_object();
    python_list list;
    obj.setattr("list", list);
    BXX_ASSERT_EQUAL(list.ref_count(), 2);
    BXX_ASSERT_EQUAL(obj.getattr<python_list>("list").m_obj, list.m_obj);
}

BXX_TEST(python_object_delattr) {
    python_object obj = create_python_object();
    python_list list;
    obj.setattr("list", list);
    obj.delattr("list");
    BXX_ASSERT_EQUAL(list.ref_count(), 1);
}

BXX_TEST(python_object_call_no_args) {
    python_object obj = eval_pyobject({
        "class TestClass:",
        "    def __init__(self):",
        "        pass",
        "    def call(self):",
        "        return 10",
        "out = TestClass()"
    });
    BXX_ASSERT_EQUAL(obj.call<int>("call"),10);
}

BXX_TEST(python_object_call_simple_args) {
    python_object obj = eval_pyobject({
        "class TestClass:",
        "    def __init__(self):",
        "        pass",
        "    def call(self,a):",
        "        return a",
        "out = TestClass()"
    });
    BXX_ASSERT_EQUAL(obj.call<int>("call",25),25);
}

BXX_TEST(python_object_call_varargs) {
    python_object obj = eval_pyobject({
        "class TestClass:",
        "    def __init__(self):",
        "        pass",
        "    def call(self,*args):",
        "        return args[0] + args[1]",
        "out = TestClass()"
    });
    BXX_ASSERT_EQUAL(obj.call<int>("call",25,10),35);
}

BXX_TEST(python_object_call_kwarg) {
    python_object obj = eval_pyobject({
        "class TestClass:",
        "    def __init__(self):",
        "        pass",
        "    def call(self,a,b):",
        "        return a",
        "out = TestClass()"
    });
    BXX_ASSERT_EQUAL(obj.call<int>("call",kwarg{"b",25}, kwarg{"a",30}), 30);
}

BXX_TEST(python_object_complex_arg) {
    python_list list;
    list.append(10);
    python_object obj = eval_pyobject({
        "class TestClass:",
        "    def __init__(self):",
        "        pass",
        "    def call(self,a):",
        "        return a",
        "out = TestClass()"
    });
    BXX_ASSERT_EQUAL(obj.call<python_list>("call",list).get<int>(0), 10);
    BXX_ASSERT_EQUAL(list.ref_count(), 1);
}

BXX_TEST(python_object_complex_kwarg) {
    python_list list;
    list.append(10);
    python_object obj = eval_pyobject({
        "class TestClass:",
        "    def __init__(self):",
        "        pass",
        "    def call(self,a,b):",
        "        return a",
        "out = TestClass()"
    });
    BXX_ASSERT_EQUAL(obj.call<python_list>("call",kwarg("b",5),kwarg("a",list)).get<int>(0), 10);
    BXX_ASSERT_EQUAL(list.ref_count(), 1);
}
