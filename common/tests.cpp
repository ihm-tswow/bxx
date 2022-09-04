#include <common/tests.hpp>
#include <common/addon.hpp>

namespace bxx
{
    void write_test_file(std::string const& localPath, char const* values, size_t size, bool binary)
    {
        fs::path path = bxx::get_addon_path() / "test_output" / localPath;
        if (!fs::exists(path.parent_path()))
        {
            fs::create_directories(path.parent_path());
        }
        std::ofstream ofs = binary ? std::ofstream(path, std::ios_base::binary) : std::ofstream(path);
        ofs.write(values, size);
    }

    void write_test_file(std::string const& localPath, std::string const& contents)
    {
        write_test_file(localPath, contents.c_str(), contents.size());
    }

    void write_test_file(std::string const& localPath, std::vector<char> contents)
    {
        write_test_file(localPath, contents.data(), contents.size(), true);
    }
}
