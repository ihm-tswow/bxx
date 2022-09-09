#include <common/script_error.hpp>
#include <common/addon.hpp>
#include <boost/stacktrace.hpp>
#include <boost/stacktrace/detail/frame_msvc.ipp>

#include <sstream>

std::string cached_library_path;

#if _WIN32
void boost::stacktrace::detail::bxx_append_library_path(com_holder<::IDebugSymbols>& idebug)
{
    std::string s;
    ULONG buffer_size = 1024;
    ULONG path_size;
    s.resize(buffer_size);
    for (;;)
    {
        idebug->GetSymbolPath(s.data(), buffer_size, &path_size);
        if (path_size < buffer_size)
        {
            break;
        }
        buffer_size *= 2;
        s.resize(buffer_size);
    }

    if (s.find(cached_library_path) == std::string::npos)
    {
        idebug->AppendSymbolPath(cached_library_path.c_str());
    }
}
#endif

static std::string format_script_error_string(std::string const& message, char const* file, char const* function, int line, boost::stacktrace::stacktrace stacktrace)
{
    std::stringstream sstream;

    sstream
        << "[Script Error]: "
        << message
        << "\n"
        << function << "(" << file << ":" << line << ")\n\n"
        << "Stack Trace:\n"
        << stacktrace
        ;

    return sstream.str();
}

namespace bxx
{
    script_error_base::script_error_base(std::string const& message, char const* file, char const* function, int line)
        : std::runtime_error(format_script_error_string(message,file,function,line,boost::stacktrace::stacktrace()))
    {}
}
