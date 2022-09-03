#pragma once

#include "class_header_builder.hpp"

#include <functional>

namespace bxx
{
    class python_object;
    class python_builder;

    enum class bl_space_type
    {
        EMPTY,
        VIEW_3D,
        IMAGE_EDITOR,
        NODE_EDITOR,
        SEQUENCE_EDITOR,
        CLIP_EDITOR,
        DOPESHEET_EDITOR,
        GRAPH_EDITOR,
        NLA_EDITOR,
        TEXT_EDITOR,
        CONSOLE,
        INFO,
        TOPBAR,
        STATUSBAR,
        OUTLINES,
        PROPERTIES,
        FILE_BROWSER,
        SPREADSHEET,
        PREFERENCES
    };

    enum class bl_region_type
    {
        WINDOW,
        HEADER,
        CHANNELS,
        TEMPORARY,
        UI,
        TOOLS,
        TOOL_PROPS,
        PREVIEW,
        HUD,
        NAVIGATION_BAR,
        EXECUTE,
        FOOTER,
        TOOL_HEADER,
        XR
    };

    // todo: incomplete
    enum class bl_context
    {
        scene,
        object,
        bone,
        data,
        material,
        image,
        objectmode
    };

    class panel_builder : public class_header_builder<panel_builder>
    {
    public:
        panel_builder(std::string const& class_name);
        ~panel_builder();
        panel_builder& set_space_type(bl_space_type type);
        panel_builder& set_region_type(bl_region_type type);
        panel_builder& set_context(bl_context context);
        panel_builder& set_draw(std::function<void(python_object, python_object)> callback);
        panel_builder& set_draw_header(std::function<void(python_object, python_object)> callback);
        panel_builder& set_poll(std::function<bool(python_object, python_object)> callback);
        void write(python_builder& builder);
    private:
        std::function<void(python_object, python_object)> m_draw;
        std::function<void(python_object, python_object)> m_draw_header;
        std::function<bool(python_object, python_object)> m_poll;
    };
}