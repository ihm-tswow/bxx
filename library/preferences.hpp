#pragma once

#include <string>

namespace bxx
{
    class preferences
    {
    public:
        static std::string get_string(std::string const& id, std::string const& def);
        static int get_int(std::string const& id, int def);
        static float get_float(std::string const& id, float def);
        static bool get_bool(std::string const& id, bool def);
    };
}
