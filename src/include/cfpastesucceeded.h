//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "cfeffect.h"

namespace msf
{

/// <summary>Collection of static helper functions to update settings in a data object.</summary>
class CCfPasteSucceeded
{
public:

    static void Set(_In_ IDataObject* pdataobject, DWORD dwEffect)
    {
        RaiseExceptionIfFailed(SetImpl(pdataobject, dwEffect));
    }

    static void SetOptional(_In_ IDataObject* pdataobject, DWORD dwEffect)
    {
        const HRESULT hr = SetImpl(pdataobject, dwEffect);
        if (FAILED(hr))
        {
            ATLTRACE2(ATL::atlTraceCOM, 0, L"CCfPasteSucceeded::SetOptional failed, hr=%x\n", hr);
        }
    }

private:

    static HRESULT SetImpl(_In_ IDataObject* pdataobject, DWORD dwEffect)
    {
        return SetClipboardFormatEffect(CFSTR_PASTESUCCEEDED, pdataobject, dwEffect);
    }
};

} // end of msf namespace
