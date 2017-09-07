//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "cfeffect.h"


namespace MSF
{


class CCfPerformedDropEffect
{
public:

    static void Set(IDataObject* pdataobject, DWORD dwEffect)
    {
        RaiseExceptionIfFailed(SetImpl(pdataobject, dwEffect));
    }


    static void SetOptional(IDataObject* pdataobject, DWORD dwEffect)
    {
        HRESULT hr = SetImpl(pdataobject, dwEffect);
        if (FAILED(hr))
        {
            ATLTRACE2(ATL::atlTraceCOM, 0, L"CCfPerformedDropEffect::SetOptional failed, hr=%x\n", hr);
        }
    }

private:

    static HRESULT SetImpl(IDataObject* pdataobject, DWORD dwEffect)
    {
        return SetCfEffect(CFSTR_PERFORMEDDROPEFFECT, pdataobject, dwEffect);
    }
};

} // end of MSF namespace
