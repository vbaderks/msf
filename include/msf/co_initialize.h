//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msf_base.h"
#include <objbase.h>

namespace msf::util
{

/// <summary>Scope based COM initialization class (based on RAII pattern).</summary>
class CoInitializer final
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

    CoInitializer(const CoInitializer&) = delete;
    CoInitializer(CoInitializer&&) = delete;
    CoInitializer& operator=(const CoInitializer&) = delete;
    CoInitializer& operator=(CoInitializer&&) = delete;
};

} // end namespace msf::util.
