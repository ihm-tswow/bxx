#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace bxx
{
    std::string get_addon_name();
    fs::path get_addon_path();
}
