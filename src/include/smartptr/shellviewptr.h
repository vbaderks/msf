//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../msfbase.h"


namespace msf
{

class IShellViewPtr : public ::IShellViewPtr
{
public:
    IShellViewPtr() :
        ::IShellViewPtr()
    {
    }


    explicit IShellViewPtr(const CLSID& clsid, IUnknown* pOuter = nullptr, DWORD dwClsContext = CLSCTX_INPROC_SERVER) :
        ::IShellViewPtr(clsid, pOuter, dwClsContext)
    {
    }


    // Purpose: Constructs a smart-pointer from any other smart pointer.
    template<typename _OtherIID>
    explicit IShellViewPtr(const _com_ptr_t<_OtherIID>& p) :
        ::IShellViewPtr(p)
    {
    }


    // Purpose: Constructs a smart-pointer from any IUnknown-based interface pointer.
    template<typename _InterfaceType>
    explicit IShellViewPtr(_InterfaceType* p) :
        ::IShellViewPtr(p)
    {
    }
};

} // end of msf namespace
