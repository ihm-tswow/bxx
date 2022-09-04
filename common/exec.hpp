#pragma once

#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
#include <fmt/core.h>

std::string join_strings(std::initializer_list<std::string> const& python);

template <typename ...Args>
void exec(fmt::format_string<Args...> str, Args... args);
void exec(std::initializer_list<std::string> lines);

template <typename ...Args>
int eval_int(fmt::format_string<Args...> str, Args... args);
int eval_int(std::initializer_list<std::string> lines);

template <typename ...Args>
float eval_float(fmt::format_string<Args...> str, Args... args);
float eval_float(std::initializer_list<std::string> lines);

template <typename ...Args>
std::string eval_string(fmt::format_string<Args...> str, Args... args);
std::string eval_string(std::initializer_list<std::string> lines);

template <typename T, typename ...Args>
T* eval_ptr(fmt::format_string<Args...> str, Args... args);

template <typename T>
T* eval_ptr(std::initializer_list<std::string> lines);

#include <common/exec.ipp>
