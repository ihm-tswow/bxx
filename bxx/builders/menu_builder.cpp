#include <bxx/builders/menu_builder.hpp>
#include <bxx/builders/python_builder.hpp>
#include <bxx/objects/python_object.hpp>
#include <bxx/objects/python_tuple.hpp>
#include <bxx/script.hpp>

namespace bxx
{
    menu_builder::menu_builder(std::string const& class_name)
        : class_header_builder<menu_builder>(class_name)
    {
        add_parent_class("bpy.types.Menu");
        set_id("BXX_MT_" + class_name);
    }

    menu_builder::~menu_builder()
    {
        python_builder builder;
        write(builder);
        builder.exec();
    }

    menu_builder& menu_builder::set_draw(std::function<void(python_object, python_object)> callback)
    {
        m_draw = callback;
        return *this;
    }

    void menu_builder::write(python_builder& builder)
    {
        auto draw = m_draw;
        size_t event_index = lib_register_event([=](python_tuple tuple)
        {
            draw(tuple.get<python_object>(0), tuple.get<python_object>(1));
            return python_object();
        });

        class_header_builder<menu_builder>::write(builder, [&]() {
            builder.begin_line("def draw(self, context):");
            auto block = builder.scoped_block(python_builder::no_brackets);
            builder.write_line("fire_event({},{},self,context)",get_script_index(), event_index);
        });
        builder.write_line("register_class({},{})", get_script_index(), get_class_name());
    }
}
