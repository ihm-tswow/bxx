#include "string_builder.hpp"

#include <sstream>

namespace bxx
{
    std::string string_to_python_string(std::string const& str)
    {
        std::stringstream sstream;
        for (char c : str)
        {
            switch (c)
            {
            case '\r': sstream << "\\r" ; break;
            case '\n': sstream << "\\n" ; break;
            case '\t': sstream << "\\t" ; break;
            case '\"': sstream << "\\\""; break;
            case '\?': sstream << "\\?" ; break;
            case '\\': sstream << "\\\\"; break;
            case '\a': sstream << "\\a" ; break;
            case '\b': sstream << "\\b" ; break;
            case '\f': sstream << "\\f" ; break;
            case '\v': sstream << "\\v" ; break;
            default: sstream << c;  break;
            }
        }
        return sstream.str();
    }
}
