//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "formatetc.h"
#include "stgmedium.h"
#include "util.h"

namespace msf
{

/// <summary>Sets the Clipboard format effect into a data object.</summary>
inline HRESULT SetClipboardFormatEffect(PCWSTR lpszFormat, _In_ IDataObject* pdataobject, DWORD dwEffect)
{
    StorageMedium stgmedium(GlobalAllocThrow(sizeof(DWORD)));

    *static_cast<DWORD*>(stgmedium.GetHGlobal()) = dwEffect;

    // Note: the shell expects fRelease to be true.
    FormatEtc formatetc(Win32::RegisterClipboardFormat(lpszFormat));
    const HRESULT hr = pdataobject->SetData(&formatetc, &stgmedium, true);
    if (SUCCEEDED(hr))
    {
        stgmedium.Detach();
    }

    return hr;
}

/// <summary>Gets the clipboard format effect from a data object.</summary>
inline HRESULT GetClipboardFormatEffect(PCWSTR lpszFormat, _In_ IDataObject* pdataobject, DWORD& dwEffect)
{
    FormatEtc formatetc(Win32::RegisterClipboardFormat(lpszFormat));
    StorageMedium medium;
    const HRESULT hr = pdataobject->GetData(&formatetc, &medium);
    if (FAILED(hr))
        return hr;

    util::GlobalLock<DWORD> globallock(medium.hGlobal);

    dwEffect = *globallock.get();
    return S_OK;
}

} // end of msf namespace
