#pragma once

#include <string>

namespace bxx
{
    bool ends_with(std::string const & value, std::string const & ending);

    enum class color_code {
        RED           = 91,
        GREEN         = 32,
        CYAN          = 36,
        GRAY          = 90,
        YELLOW        = 93,
        DEFAULT       = 39,
    };

    std::ostream& operator<<(std::ostream& os, color_code const& color);

    bool match(std::string const& pattern, std::string const& candidate);
}