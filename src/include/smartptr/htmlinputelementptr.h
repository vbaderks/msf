//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../msfbase.h"
#include <mshtml.h>


_COM_SMARTPTR_TYPEDEF(IHTMLInputElement, __uuidof(IHTMLInputElement));


namespace MSF
{

class IHTMLInputElementPtr : public ::IHTMLInputElementPtr
{
public:
    // Constructs a smart-pointer from any other smart pointer.
    //
    template<typename _OtherIID> IHTMLInputElementPtr(const _com_ptr_t<_OtherIID>& p) :
        ::IHTMLInputElementPtr(p)
    {
    }


    // Constructs a smart-pointer from any IUnknown-based interface pointer.
    //
    template<typename _InterfaceType> IHTMLInputElementPtr(_InterfaceType* p) :
        ::IHTMLInputElementPtr(p)
    {
    }


    bstr_t GetName()
    {
        bstr_t name;
        HRESULT hr = GetInterfacePtr()->get_name(name.GetAddress());
        RaiseExceptionIfFailed(hr);
        return name;
    }


    bstr_t GetType()
    {
        bstr_t type;
        HRESULT hr = GetInterfacePtr()->get_type(type.GetAddress());
        RaiseExceptionIfFailed(hr);
        return type;
    }


    void SetValue(bstr_t bstrValue)
    {
        HRESULT hr = GetInterfacePtr()->put_value(bstrValue);
        RaiseExceptionIfFailed(hr);
    }
};

}
