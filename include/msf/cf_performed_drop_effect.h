//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "cf_effect.h"

namespace msf
{

class CCfPerformedDropEffect
{
public:
    static void Set(IDataObject* dataObject, DWORD effect)
    {
        RaiseExceptionIfFailed(SetImpl(dataObject, effect));
    }

    static void SetOptional(IDataObject* dataObject, DWORD effect)
    {
        const HRESULT result = SetImpl(dataObject, effect);
        if (FAILED(result))
        {
            ATLTRACE2(ATL::atlTraceCOM, 0, L"CCfPerformedDropEffect::SetOptional failed, result=%x\n", result);
        }
    }

private:
    static HRESULT SetImpl(IDataObject* dataObject, DWORD effect)
    {
        return SetClipboardFormatEffect(CFSTR_PERFORMEDDROPEFFECT, dataObject, effect);
    }
};

} // end of msf namespace
