//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "shelluuids.h"


_COM_SMARTPTR_TYPEDEF(IQueryInfo, __uuidof(IQueryInfo));


namespace MSF
{

class IQueryInfoPtr : public ::IQueryInfoPtr
{
public:
    explicit IQueryInfoPtr(const CLSID& clsid, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_INPROC_SERVER) :
        ::IQueryInfoPtr(clsid, pOuter, dwClsContext)
    {
    }


    // Purpose: Constructs a smart-pointer from any other smart pointer.
    template<typename _OtherIID> IQueryInfoPtr(const _com_ptr_t<_OtherIID>& p) :
        ::IQueryInfoPtr(p)
    {
    }


    // Purpose: Constructs a smart-pointer from any IUnknown-based interface pointer.
    template<typename _InterfaceType> IQueryInfoPtr(_InterfaceType* p) :
        ::IQueryInfoPtr(p)
    {
    }


    CStringW GetInfoTip(DWORD dwFlags = 0)
    {
        LPWSTR pwszInfoTip;
        RaiseExceptionIfFailed(GetInterfacePtr()->GetInfoTip(dwFlags, &pwszInfoTip));
    
        CStringW strInfoTip(pwszInfoTip);

        CoTaskMemFree(pwszInfoTip);

        return strInfoTip;
    }


    DWORD GetInfoFlags()
    {
        DWORD dwFlags;
        RaiseExceptionIfFailed(GetInterfacePtr()->GetInfoFlags(&dwFlags));

        return dwFlags;
    }
};

} // end of MSF namespace
