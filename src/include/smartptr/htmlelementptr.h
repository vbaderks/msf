//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../msfbase.h"
#include <mshtml.h>

_COM_SMARTPTR_TYPEDEF(IHTMLElement, __uuidof(IHTMLElement));


namespace msf
{

class IHTMLElementPtr : public ::IHTMLElementPtr
{
public:
    // Constructs a smart-pointer from any other smart pointer.
    //
    template<typename _OtherIID> IHTMLElementPtr(const _com_ptr_t<_OtherIID>& p) :
        ::IHTMLElementPtr(p)
    {
    }


    // Constructs a smart-pointer from any IUnknown-based interface pointer.
    //
    template<typename _InterfaceType> IHTMLElementPtr(_InterfaceType* p) :
        ::IHTMLElementPtr(p)
    {
    }


    void Click()
    {
        HRESULT hr = GetInterfacePtr()->click();
        RaiseExceptionIfFailed(hr);
    }
};

}
