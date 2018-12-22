//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "cf_effect.h"

namespace msf
{

/// <summary>Collection of static helper functions to update settings in a data object.</summary>
class CfPasteSucceeded
{
public:
    static void Set(_In_ IDataObject* dataObject, DWORD effect)
    {
        RaiseExceptionIfFailed(SetImpl(dataObject, effect));
    }

    static void SetOptional(_In_ IDataObject* dataObject, DWORD effect)
    {
        const HRESULT result = SetImpl(dataObject, effect);
        if (FAILED(result))
        {
            ATLTRACE(L"CfPasteSucceeded::SetOptional failed, result=%x\n", result);
        }
    }

private:
    static HRESULT SetImpl(_In_ IDataObject* dataObject, DWORD effect)
    {
        return SetClipboardFormatEffect(CFSTR_PASTESUCCEEDED, dataObject, effect);
    }
};

} // end of msf namespace
