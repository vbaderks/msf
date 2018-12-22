//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "cf_effect.h"

namespace msf
{

class CfPreferredDropEffect
{
public:

    static DWORD Get(IDataObject* dataObject)
    {
        DWORD effect;
        RaiseExceptionIfFailed(GetClipboardFormatEffect(CFSTR_PREFERREDDROPEFFECT, dataObject, effect));

        return effect;
    }

    static DWORD GetOptional(IDataObject* dataObject, DWORD dwEffectDefault = DROPEFFECT_NONE)
    {
        DWORD effect;

        const HRESULT result = GetClipboardFormatEffect(CFSTR_PREFERREDDROPEFFECT, dataObject, effect);
        if (SUCCEEDED(result))
            return effect;

        ATLTRACE2(ATL::atlTraceCOM, 0, L"CCfPreferredDropEffect::GetOptional failed, hr=%x\n", result);
        return dwEffectDefault;
    }

    static void Set(IDataObject* dataObject, DWORD effect)
    {
        RaiseExceptionIfFailed(SetClipboardFormatEffect(CFSTR_PREFERREDDROPEFFECT, dataObject, effect));
    }
};

} // end of msf namespace
