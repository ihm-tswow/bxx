#include "addon_preferences_builder.hpp"

#include "python_object.hpp"
#include "python_builder.hpp"
#include "addon.hpp"
#include "../script.hpp"

namespace bxx
{
    addon_preferences_builder::addon_preferences_builder(std::string const& classname)
        : class_header_builder<addon_preferences_builder>(classname)
    {
        add_parent_class("bpy.types.AddonPreferences");
        set_id(get_addon_name());
    }

    addon_preferences_builder::~addon_preferences_builder()
    {
        python_builder builder;
        write(builder);
        builder.exec();
    }

    addon_preferences_builder& addon_preferences_builder::set_draw(std::function<void(python_object, python_object)> draw)
    {
        m_draw = draw;
        return *this;
    }

    void addon_preferences_builder::write(python_builder& builder)
    {
        class_header_builder<addon_preferences_builder>::write(builder, [&]() {
            property_builder<addon_preferences_builder>::write(builder);
            if (m_draw)
            {
                auto draw = m_draw;
                size_t event_id = lib_register_event([draw](python_tuple const& tuple) {
                    draw(tuple.get<python_object>(0), tuple.get<python_object>(1));
                    return python_object();
                });
                builder.begin_line("def draw(self,ctx):");
                auto block = builder.scoped_block(python_builder::no_brackets);
                builder.write_line("fire_event({},{},self,ctx)", get_script_index(), event_id);
            }
        });
        builder.write_line("register_class({},{})", get_script_index(), get_class_name());
    }
}
