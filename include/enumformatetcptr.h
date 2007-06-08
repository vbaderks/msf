//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace MSF
{

class IEnumFORMATETCPtr : public ::IEnumFORMATETCPtr
{
public:
    IEnumFORMATETCPtr() : ::IEnumFORMATETCPtr()
    {
    }


    bool Next(FORMATETC& formatetc)
    {
        return Next(1, &formatetc, NULL);
    }

    
    bool Next(ULONG celt, FORMATETC* rgelt, ULONG* pceltFetched)
    {
        HRESULT hr = GetInterfacePtr()->Next(celt, rgelt, pceltFetched);
        RaiseExceptionIfFailed(hr);

        return hr == S_OK;
    }
};


} // end MSF namespace
