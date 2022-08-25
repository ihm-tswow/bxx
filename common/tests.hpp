#pragma once

#include "util.hpp"

#include <string>
#include <exception>
#include <iostream>
#include <filesystem>
#include <ostream>
#include <fstream>

#define BXX_TEST(name) void name()
#define BXX_ASSERT_EQUAL(v1,v2) bxx::_assert_equal(__FILE__, __func__, __LINE__, v1, v2)
#define BXX_ASSERT_NOT_EQUAL(v1,v2) bxx::_assert_not_equal(__FILE__, __func__, __LINE__, v1, v2)
#define BXX_ASSERT(val) bxx::_assert_true(__FILE__, __func__, __LINE__, val)

namespace fs = std::filesystem;

namespace bxx
{
    class test_exception : public std::exception {};

    struct test_case
    {
        char const* m_name;
        void (*test_ptr)();
    };

    struct test_collection
    {
        test_case* m_entries;
        int m_count;
    };

    template <typename Arg, typename... Args>
    void _print_test_error(std::string const& file, std::string const& func, size_t lineno, Arg&& arg, Args&&...args)
    {
        static fs::path addon_root = fs::path(__FILE__)
            .parent_path()
            .parent_path()
            .parent_path()
            ;
        static fs::path relative_path = fs::relative(file, addon_root);

        std::cout
            << "\n"
            << bxx::color_code::RED
            << "[Fail] "
            << bxx::color_code::GRAY
            << relative_path.string() << ":" << func << "(" << lineno << "): "
            << bxx::color_code::CYAN
            << std::forward<Arg>(arg)
            ;
        ((std::cout << std::forward<Args>(args)), ...);
        std::cout
            << bxx::color_code::DEFAULT
            << "\n";
        throw test_exception();
    }

    inline void _assert_true(char const* file, char const* func, size_t lineno, bool value)
    {
        if (!value)
        {
            _print_test_error(file, func, lineno, "false");
        }
    }

    template <typename T, typename U>
    inline void _assert_equal(std::string const& file, std::string const& func, size_t lineno, T const& v1, U const& v2)
    {
        if (v1 != v2)
        {
            _print_test_error(file, func, lineno, v1, " != ", v2);
        }
    }

    template <typename T, typename U>
    inline void _assert_not_equal(std::string const& file, std::string const& func, size_t lineno, T const& v1, U const& v2)
    {
        if (v1 == v2)
        {
            _print_test_error(file, func, lineno, v1, " == ", v2);
        }
    }

    void write_test_file(std::string const& localPath, char const* values, size_t size, bool binary = false);
    void write_test_file(std::string const& localPath, std::string const& contents);
    void write_test_file(std::string const& localPath, std::vector<char> contents);
}