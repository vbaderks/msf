//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#pragma once

#include "msfbase.h"
#include "olestring.h"
#include "updateregistry.h"

namespace msf
{

// Note: ICopyHook is a #define. It stands for ICopyHookA or ICopyHookW.
//       The shell will query both interfaces and use the one that is returned.
//       For Win NT the preferred interface is ICopyHookW, this can be achieved by
//       compiling with UNICODE defines.

// Comment: ICopyHook is a bad example of a OLE\COM interface. It doesn't return a HRESULT
//          and it doesn't use wchar_t to pass a string. It even requires 2 interface
//          to handle the ANSI and UNICODE model.

template <typename T>
class __declspec(novtable) CopyHookImpl :
    public ICopyHook
{
public:
    static HRESULT __stdcall UpdateRegistry(BOOL registerObject, UINT resourceId,
        PCWSTR description, PCWSTR copyHookName) noexcept
    {
        OleString classId;
        StringFromCLSID(T::GetObjectCLSID(), classId);

        ATL::_ATL_REGMAP_ENTRY entries[] =
        {
            {L"CLSID", classId},
            { L"DESCRIPTION", description },
            { L"COPYHOOKNAME", copyHookName },
            { nullptr, nullptr }
        };

        return ATL::_pAtlModule->UpdateRegistryFromResource(resourceId, registerObject, entries);
    }

    CopyHookImpl(const CopyHookImpl&) = delete;
    CopyHookImpl(CopyHookImpl&&) = delete;
    CopyHookImpl& operator=(const CopyHookImpl&) = delete;
    CopyHookImpl& operator=(CopyHookImpl&&) = delete;

protected:
    CopyHookImpl() noexcept
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ICopyHookImpl::Constructor (instance=%p)\n", this);
    }


    ~CopyHookImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ICopyHookImpl::~ICopyHookImpl (instance=%p)\n", this);
    }

    // ICopyHookImpl only consists of 1 function. So there is little to provide.
    // As a derived class, just implement the CopyCallback function.
};

} // end namespace msf
