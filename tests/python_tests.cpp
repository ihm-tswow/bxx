#include "tests.hpp"

#include "python_object.hpp"

BXX_TEST(empty_python_object_init_to_none) {
    BXX_ASSERT_EQUAL(bxx::python_object().m_obj, Py_None);
}

BXX_TEST(create_py_references) {
    bxx::python_list list;
    {
        bxx::py_ref<bxx::python_list> ref = bxx::create_python_list(0);
        list = ref.m_value;
        BXX_ASSERT_EQUAL(ref->ref_count(), 1);
        BXX_ASSERT_EQUAL(list.ref_count(), 1);
    }
    BXX_ASSERT_EQUAL(list.ref_count(), 0);
}

BXX_TEST(copy_py_references) {
    bxx::python_list list;
    {
        bxx::py_ref<bxx::python_list> ref1 = bxx::create_python_list(0);
        list = ref1.m_value;
        bxx::py_ref<bxx::python_list> ref2(ref1);
        BXX_ASSERT_EQUAL(ref1->ref_count(), 2);
        BXX_ASSERT_EQUAL(list.ref_count(), 2);
    }
    BXX_ASSERT_EQUAL(list.ref_count(), 0);
}

BXX_TEST(list_append) {
    bxx::py_ref<bxx::python_list> list = bxx::create_python_list(0);
    list->append(10);
    BXX_ASSERT_EQUAL(list->get<int>(0), 10);
}

BXX_TEST(list_set) {
    bxx::py_ref<bxx::python_list> list = bxx::create_python_list(1);
    list->set(0, 10);
    BXX_ASSERT_EQUAL(list->get<int>(0), 10);
}

BXX_TEST(tuple_set) {
    bxx::py_ref<bxx::python_tuple> list = bxx::create_python_tuple(1);
    list->set(0, 10);
    BXX_ASSERT_EQUAL(list->get<int>(0), 10);
}

BXX_TEST(dict_set) {
    bxx::py_ref<bxx::python_dict> dict = bxx::create_python_dict();
    dict->set("key", 10);
    BXX_ASSERT_EQUAL(dict->get<int>("key"), 10);
}

BXX_TEST(setattr) {
    bxx::py_ref<bxx::python_tuple> tup = bxx::create_python_tuple(1);
    tup->setattr("key", 10);
    BXX_ASSERT_EQUAL(tup->getattr<int>("key"), 10);
}

BXX_TEST(insert_complex) {
    bxx::py_ref<bxx::python_list> list = bxx::create_python_list(0);
    bxx::py_ref<bxx::python_list> list2 = bxx::create_python_list(0);
    list2->append(10);
    BXX_ASSERT_EQUAL(list2->ref_count(), 1);
    list->append(list2);
    BXX_ASSERT_EQUAL(list2->ref_count(), 2);
    BXX_ASSERT_EQUAL(list->get<bxx::python_list>(0)->get<int>(0),10);
}
