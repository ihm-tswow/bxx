#include <bxx/script_error.hpp>
#include <boost/stacktrace.hpp>
#include <boost/stacktrace/detail/frame_msvc.ipp>

#include <iostream>

#if _WIN32
void boost::stacktrace::detail::bxx_append_library_path(com_holder<::IDebugSymbols>& idebug)
{
}
#endif
