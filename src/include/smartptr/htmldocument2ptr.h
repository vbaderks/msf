//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "../msfbase.h"
#include <mshtml.h>

_COM_SMARTPTR_TYPEDEF(IHTMLDocument2, __uuidof(IHTMLDocument2));


namespace msf
{

class IHTMLDocument2Ptr : public ::IHTMLDocument2Ptr
{
public:
	// Constructs a smart-pointer from any other smart pointer.
	//
	template<typename _OtherIID> IHTMLDocument2Ptr(const _com_ptr_t<_OtherIID>& p) :
		::IHTMLDocument2Ptr(p)
	{
	}


	// Constructs a smart-pointer from any IUnknown-based interface pointer.
	//
	template<typename _InterfaceType> IHTMLDocument2Ptr(_InterfaceType* p) :
		::IHTMLDocument2Ptr(p)
	{
	}


	bstr_t GetUrl()
	{
		bstr_t url;
		RaiseExceptionIfFailed(GetInterfacePtr()->get_URL(url.GetAddress()));
		return url;
	}
};

}
