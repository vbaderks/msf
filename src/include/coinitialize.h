//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
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
};

} // end namespace msf::util.
