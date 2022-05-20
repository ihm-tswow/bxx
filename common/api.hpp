#pragma once

#if defined(WIN32) || defined (_WIN32) || defined(__WIN32)
    #define BXX_API extern "C" __declspec(dllexport)
#else
    #define BXX_API extern "C" __attribute__((visibility("default")))
#endif