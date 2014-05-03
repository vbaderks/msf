//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include <comdef.h>
#include "persistfolderptr.h"


namespace MSF
{

class IShellBrowserPtr : public ::IShellBrowserPtr
{
public:
    explicit IShellBrowserPtr(const CLSID& clsid, IUnknown* pOuter = nullptr, DWORD dwClsContext = CLSCTX_INPROC_SERVER) :
        ::IShellBrowserPtr(clsid, pOuter, dwClsContext)
    {
    }


    // Purpose: Constructs a smart-pointer from any other smart pointer.
    template<typename _OtherIID> IShellBrowserPtr(const _com_ptr_t<_OtherIID>& p) :
        ::IShellBrowserPtr(p)
    {
    }


    // Purpose: Constructs a smart-pointer from any IUnknown-based interface pointer.
    template<typename _InterfaceType> IShellBrowserPtr(_InterfaceType* p) :
        ::IShellBrowserPtr(p)
    {
    }


    void BrowseObject(LPCITEMIDLIST pidl, UINT wFlags)
    {
        RaiseExceptionIfFailed(GetInterfacePtr()->BrowseObject(pidl, wFlags));
    }
};

} // end of MSF namespace
