//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include <shlobj.h>
#include "shelluuids.h"
#include "olestring.h"
#include "updateregistry.h"

namespace MSF
{

// Note: ICopyHook is a #define. It stands for ICopyHookA or ICopyHookW. 
//       The shell will query both interfaces and use the one that is returned.
//       For Win NT the prefered interface is ICopyHookW, this can be achived by 
//       compiling with UNICODE defines.

// Comment: ICopyHook is a bad example of a OLE\COM interface. It doesn't return a HRESULT
//          and it doesn't use wchar_t to pass a string. It even requires 2 interface
//          to handle the ANSI and UNICODE model.

template <typename T>
class ATL_NO_VTABLE ICopyHookImpl :
    public ICopyHook
{
public:
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, UINT nResId,
        PCWSTR szDescription, PCWSTR szCopyHookName) throw()
    {
        COleString olestrCLSID;
        StringFromCLSID(T::GetObjectCLSID(), olestrCLSID);

        _ATL_REGMAP_ENTRY regmapEntries[] =
        {
            { L"CLSID", olestrCLSID },
            { L"DESCRIPTION", szDescription },
            { L"COPYHOOKNAME", szCopyHookName },
            { nullptr, nullptr }
        };

        return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
    }

    ICopyHookImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"ICopyHookImpl::Constructor (instance=%p)\n", this);
    }

    ~ICopyHookImpl() throw()
    {
        ATLTRACE2(atlTraceCOM, 0, L"ICopyHookImpl::~ICopyHookImpl (instance=%p)\n", this);
    }

    // ICopyHookImpl only consists of 1 function. So there is little to provide.
    // As a derived class, just implement the CopyCallback function.
};

} // end namespace MSF
