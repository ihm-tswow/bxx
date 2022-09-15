#include <bxx/enums.hpp>

#include <magic_enum.hpp>

namespace bxx::enums
{
    template <typename T>
    std::string get_enum_name(T value)
    {
        return std::string(magic_enum::enum_name<T>(value));
    }

    template <typename T>
    std::vector<std::pair<T,std::string>> get_enum_entries()
    {
        auto enums = magic_enum::enum_values<T>();
        std::vector<std::pair<T,std::string>> enums_out;
        enums_out.reserve(enums.size());
        for (size_t i = 0; i < enums.size(); ++i)
        {
            enums_out.push_back(std::make_pair(enums[i], get_enum_name<T>(enums[i])));
        }
        return enums_out;
    }

    template <typename T>
    T get_enum_value(std::string const& name)
    {
        auto val = magic_enum::enum_cast<T>(name);
        BXX_SCRIPT_ASSERT(val.has_value(), enum_cast_error, "Invalid enum key: {}", name.c_str());
        return val.value();
    }

    template <typename T>
    enum_meta get_enum_meta(T value)
    {
        return enum_meta{ get_enum_name<T>(value),"",""};
    }
}