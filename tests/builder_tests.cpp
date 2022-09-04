#include <bxx/builders/class_header_builder.hpp>
#include <bxx/builders/value_builder.hpp>
#include <bxx/builders/python_builder.hpp>
#include <bxx/builders/property_builder.hpp>
#include <bxx/builders/operator_builder.hpp>
#include <common/tests.hpp>
#include <common/addon.hpp>

#define ASSERT_VALUE_CODE(value,code)\
    {\
        bxx::python_builder builder;\
        bxx::builder_value(value).write(builder);\
        BXX_ASSERT_EQUAL(builder.get_code(),code);\
    }\

#define ASSERT_VALUE_TYPE(value,type)\
    BXX_ASSERT(bxx::builder_value(value).is<type>());

BXX_TEST(string_initialization) { ASSERT_VALUE_TYPE("hello", std::string) }
BXX_TEST(int_initialization) { ASSERT_VALUE_TYPE(25.0, double) }
BXX_TEST(double_initialization) { ASSERT_VALUE_TYPE(25.0, double) }
BXX_TEST(bool_initialization) { ASSERT_VALUE_TYPE(true, bool) }
BXX_TEST(map_initialization) { ASSERT_VALUE_TYPE([](bxx::map_builder&) {}, std::unique_ptr<bxx::map_builder>) }
BXX_TEST(set_initialization) { ASSERT_VALUE_TYPE([](bxx::set_builder&) {}, std::unique_ptr<bxx::set_builder>) }
BXX_TEST(list_initialization) { ASSERT_VALUE_TYPE([](bxx::list_builder&) {}, std::unique_ptr<bxx::list_builder>) }

BXX_TEST(string_serialization) { ASSERT_VALUE_CODE("hello","\"hello\"") }
BXX_TEST(double_serialization) { ASSERT_VALUE_CODE(25,"25") }
BXX_TEST(bool_serialization) { ASSERT_VALUE_CODE(true,"True") }

BXX_TEST(value_write_test) {
    bxx::python_builder builder;
    bxx::map_builder()
        .set("number", 25)
        .set("string", "value")
        .set("bool", true)
        .set("set", [](bxx::set_builder& set) { set
            .insert(10)
            .insert(20)
            .insert(30)
            ;
        })
        .set("list",[](bxx::list_builder& set){ set
            .add(10)
            .add(20)
            .add(30)
            ;
        })
        .set("map",[](bxx::map_builder& map){ map
            .set("10",10)
            .set("20",20)
            .set("30",30)
            ;
        })
        .write(builder)
        ;
    bxx::write_test_file("map_builder_output.txt", builder.get_code());
}

BXX_TEST(property_write_test) {
    bxx::python_builder builder;
    class DummyBuilder : public bxx::property_builder<DummyBuilder> { };

    DummyBuilder()
        .add_string_property("string_id", "String ID", "String Description")
        .add_bool_property("bool_id", "Bool ID", "Bool Description")
        .add_enum_property("enum.id", "Enum ID", { bxx::enum_entry{"ENUM_1"}, bxx::enum_entry{"ENUM_2"} }, "Enum Description")
        .add_bool_property("float.id", "Float ID", "Float Description")
        .write(builder)
        ;
    bxx::write_test_file("property_builder_output.txt", builder.get_code());
}

BXX_TEST(class_header_write_test) {
    bxx::python_builder builder;
    class DummyBuilder : public bxx::class_header_builder<DummyBuilder> {
    public:
        DummyBuilder(std::string const& name)
            : bxx::class_header_builder<DummyBuilder>(name)
        {}
    };

    DummyBuilder("no_parent")
        .set_class_variable("number",25)
        .set_class_variable("string","value")
        .set_class_variable("bool",true)
        .write(builder, [&]() { builder.write_line("# Inside Class"); })
        ;

    DummyBuilder("single_parent")
        .add_parent_class("parent1")
        .write(builder, [&]() { builder.write_line("# Inside Class"); })
        ;

    DummyBuilder("multi_parent")
        .add_parent_class("parent1")
        .add_parent_class("parent2")
        .write(builder, [&]() { builder.write_line("# Inside Class"); })
        ;
    bxx::write_test_file("class_builder_output.txt", builder.get_code());
}

BXX_TEST(operator_write_test) {
    bxx::python_builder builder;
    bxx::operator_builder("empty_operator",false)
        .write(builder)
        ;

    bxx::operator_builder("single_property",false)
        .add_string_property("my_string","My String","default value","My String Description")
        .write(builder)
        ;
    bxx::write_test_file("operator_builder_output.txt",builder.get_code());
}
