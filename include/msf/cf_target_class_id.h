//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "format_etc.h"
#include "global_lock.h"
#include "stg_medium.h"

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
        if (const HRESULT result = dataObject->GetData(&formatEtc, &medium); FAILED(result))
            return result;

        const util::GlobalLock<CLSID> globalLock(medium.hGlobal);

        clsid = *globalLock.get();
        return S_OK;
    }
};

} // end of msf namespace
