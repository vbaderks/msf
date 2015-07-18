//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "..\ibrowserframeoptions.h"

_COM_SMARTPTR_TYPEDEF(IBrowserFrameOptions, __uuidof(IBrowserFrameOptions));


namespace MSF
{

class IBrowserFrameOptionsPtr : public ::IBrowserFrameOptionsPtr
{
public:
    explicit IBrowserFrameOptionsPtr(const CLSID& clsid, IUnknown* pOuter = nullptr, DWORD dwClsContext = CLSCTX_INPROC_SERVER) :
        ::IBrowserFrameOptionsPtr(clsid, pOuter, dwClsContext)
    {
    }


    // Constructs a smart-pointer from any other smart pointer.
    //
    template<typename _OtherIID> IBrowserFrameOptionsPtr(const _com_ptr_t<_OtherIID>& p) :
        ::IBrowserFrameOptionsPtr(p)
    {
    }


    // Constructs a smart-pointer from any IUnknown-based interface pointer.
    //
    template<typename _InterfaceType> IBrowserFrameOptionsPtr(_InterfaceType* p) :
        ::IBrowserFrameOptionsPtr(p)
    {
    }


    BROWSERFRAMEOPTIONS GetFrameOptions(BROWSERFRAMEOPTIONS dwMask)
    {
        BROWSERFRAMEOPTIONS dwOptions;
        HRESULT hr = GetInterfacePtr()->GetFrameOptions(dwMask, &dwOptions);
        if (FAILED(hr))
            _com_raise_error(hr);

        return dwOptions;
    }
};

} // end of MSF namespace
