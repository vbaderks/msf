//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#if (!defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0400))
    #error WIN32_NT must be at least version 4.0 (0x400) to access CoInitaliseEx
#endif

#include "msfbase.h"
#include <objbase.h>

namespace MSF
{
namespace util
{

/// <summary>Scope based COM initialization class (based on RAII pattern).</summary>
class CoInitializer
{
public:
    CoInitializer()
    {
        ATLVERIFY(SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)));
    }

    ~CoInitializer()
    {
        CoUninitialize();
    }
};

}  // end namespace util.
} // end namespace MSF.
