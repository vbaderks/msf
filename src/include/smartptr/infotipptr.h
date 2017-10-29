//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../msfbase.h"
#include "queryinfoptr.h"
#include "persistfileptr.h"


namespace msf
{

// An infotip has 2 main COM interfaces. Use IUnknown as the base interface to hold
// a reference on the general object.
class IInfoTipPtr : public IUnknownPtr
{
public:
    explicit IInfoTipPtr(const CLSID& clsid, IUnknown* pOuter = nullptr, DWORD dwClsContext = CLSCTX_INPROC_SERVER) :
        IUnknownPtr(clsid, pOuter, dwClsContext)
    {
    }


    void Load(PCWSTR wszFilename)
    {
        msf::IPersistFilePtr persistfile(this);

        persistfile.Load(wszFilename);
    }


    CStringW GetInfoTip()
    {
        msf::IQueryInfoPtr queryinfo(this);

        return queryinfo.GetInfoTip();
    }


    CStringW GetInfoTip(PCWSTR wszFilename)
    {
        Load(wszFilename);

        return GetInfoTip();
    }
};

} // end msf namespace
