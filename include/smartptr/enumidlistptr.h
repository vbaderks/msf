//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace MSF
{

class IEnumIDListPtr : public ::IEnumIDListPtr
{
public:
    IEnumIDListPtr() :
        ::IEnumIDListPtr()
    {
    }


    explicit IEnumIDListPtr(const CLSID& clsid, IUnknown* pOuter = nullptr, DWORD dwClsContext = CLSCTX_INPROC_SERVER) :
        ::IEnumIDListPtr(clsid, pOuter, dwClsContext)
    {
    }


    // Purpose: Constructs a smart-pointer from any other smart pointer.
    template<typename _OtherIID> IEnumIDListPtr(const _com_ptr_t<_OtherIID>& p) :
        ::IEnumIDListPtr(p)
    {
    }


    // Purpose: Constructs a smart-pointer from any IUnknown-based interface pointer.
    template<typename _InterfaceType> IEnumIDListPtr(_InterfaceType* p) :
        ::IEnumIDListPtr(p)
    {
    }


    bool Next(LPITEMIDLIST* ppidl)
    {
        HRESULT hr = GetInterfacePtr()->Next(1, ppidl, nullptr);
        RaiseExceptionIfFailed(hr);

        return hr == S_OK;
    }
};

} // end of MSF namespace
