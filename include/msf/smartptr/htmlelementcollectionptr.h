//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "../msfbase.h"
#include <mshtml.h>
#include "../macros.h"

_COM_SMARTPTR_TYPEDEF(IHTMLElementCollection, __uuidof(IHTMLElementCollection));


namespace msf
{

class IHTMLElementCollectionPtr : public ::IHTMLElementCollectionPtr
{
public:
    // Default constructor.
    //
    IHTMLElementCollectionPtr() noexcept :
      ::IHTMLElementCollectionPtr()
    {
    }


    // Constructs a smart-pointer from any other smart pointer.
    //
    template<typename _OtherIID> IHTMLElementCollectionPtr(const _com_ptr_t<_OtherIID>& p) :
        ::IHTMLElementCollectionPtr(p)
    {
    }


    // Constructs a smart-pointer from any IUnknown-based interface pointer.
    //
    template<typename _InterfaceType> IHTMLElementCollectionPtr(_InterfaceType* p) :
        ::IHTMLElementCollectionPtr(p)
    {
    }


    long GetLength() const
    {
        long length;
        HRESULT hr = GetInterfacePtr()->get_length(&length);
        RaiseExceptionIfFailed(hr);
        return length;
    }


    IDispatchPtr Item(long index, long collectionIndex = 0) const
    {
        IDispatchPtr rdispElement;
        HRESULT hr = GetInterfacePtr()->item(_variant_t(index), _variant_t(collectionIndex), &rdispElement);
        RaiseExceptionIfFailed(hr);
        return rdispElement;
    }
};

}
