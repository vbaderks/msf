//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "cfeffect.h"

namespace msf
{

class CCfPreferredDropEffect
{
public:

    static DWORD Get(IDataObject* pdataobject)
    {
        DWORD dwEffect;
        RaiseExceptionIfFailed(GetClipboardFormatEffect(CFSTR_PREFERREDDROPEFFECT, pdataobject, dwEffect));

        return dwEffect;
    }

    static DWORD GetOptional(IDataObject* pdataobject, DWORD dwEffectDefault = DROPEFFECT_NONE)
    {
        DWORD dwEffect;

        const HRESULT hr = GetClipboardFormatEffect(CFSTR_PREFERREDDROPEFFECT, pdataobject, dwEffect);
        if (SUCCEEDED(hr))
            return dwEffect;

        ATLTRACE2(ATL::atlTraceCOM, 0, L"CCfPreferredDropEffect::GetOptional failed, hr=%x\n", hr);
        return dwEffectDefault;
    }

    static void Set(IDataObject* pdataobject, DWORD dwEffect)
    {
        RaiseExceptionIfFailed(SetClipboardFormatEffect(CFSTR_PREFERREDDROPEFFECT, pdataobject, dwEffect));
    }
};

} // end of msf namespace
