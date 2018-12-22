//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "format_etc.h"
#include "stg_medium.h"
#include "util.h"

namespace msf
{

/// <summary>Sets the Clipboard format effect into a data object.</summary>
inline HRESULT SetClipboardFormatEffect(PCWSTR format, _In_ IDataObject* dataObject, DWORD effect)
{
    StorageMedium medium{GlobalAllocThrow(sizeof(DWORD))};

    *static_cast<DWORD*>(medium.GetHGlobal()) = effect;

    // Note: the shell expects fRelease to be true.
    FormatEtc formatEtc{Win32::RegisterClipboardFormat(format)};
    const HRESULT result = dataObject->SetData(&formatEtc, &medium, true);
    if (SUCCEEDED(result))
    {
        medium.Detach();
    }

    return result;
}

/// <summary>Gets the clipboard format effect from a data object.</summary>
inline HRESULT GetClipboardFormatEffect(PCWSTR format, _In_ IDataObject* dataObject, DWORD& effect)
{
    FormatEtc formatEtc{Win32::RegisterClipboardFormat(format)};
    StorageMedium medium;
    const HRESULT result = dataObject->GetData(&formatEtc, &medium);
    if (FAILED(result))
        return result;

    util::GlobalLock<DWORD> globalLock{medium.hGlobal};

    effect = *globalLock.get();
    return S_OK;
}

} // end of msf namespace
