#include "util.hpp"
#include <ostream>

bool bxx::ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::ostream& bxx::operator<<(std::ostream& os, bxx::color_code const& color)
{
    os << "\033[" << int(color) << "m";
    return os;
}

static bool match(const char* pattern, const char* candidate, int p, int c) {
    if (pattern[p] == '\0') {
        return candidate[c] == '\0';
    }
    else if (pattern[p] == '*') {
        for (; candidate[c] != '\0'; c++) {
            if (match(pattern, candidate, p + 1, c))
                return true;
        }
        return match(pattern, candidate, p + 1, c);
    }
    else {
        return match(pattern, candidate, p + 1, c + 1);
    }
}


bool bxx::match(std::string const& pattern, std::string const& candidate)
{
    return ::match(pattern.c_str(), candidate.c_str(), 0, 0);
}

