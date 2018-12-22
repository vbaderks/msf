//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "formatetc.h"


namespace msf
{

class CCfTargetCLSID final
{
public:

    static void Get(IDataObject* dataObject, CLSID& clsid)
    {
        RaiseExceptionIfFailed(GetImpl(dataObject, clsid));
    }


    static bool GetOptional(IDataObject* dataObject, CLSID& clsid)
    {
        const HRESULT result = GetImpl(dataObject, clsid);
        return SUCCEEDED(result);
    }

private:

    static HRESULT GetImpl(IDataObject* dataObject, CLSID& clsid)
    {
        FormatEtc formatEtc(CFSTR_TARGETCLSID);
        StorageMedium medium;
        const HRESULT result = dataObject->GetData(&formatEtc, &medium);
        if (FAILED(result))
            return result;

        GlobalLock<CLSID> globallock(medium.hGlobal);

        clsid = *globallock.get();
        return S_OK;
    }
};

} // end of msf namespace
