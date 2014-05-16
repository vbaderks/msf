//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include <mshtml.h>
#include "macros.h"
#include "htmlelementcollectionptr.h"

_COM_SMARTPTR_TYPEDEF(IHTMLDocument3, __uuidof(IHTMLDocument3));


namespace MSF
{

class IHTMLDocument3Ptr : public ::IHTMLDocument3Ptr
{
public:
	// Constructs a smart-pointer from any other smart pointer.
	//
	template<typename _OtherIID> IHTMLDocument3Ptr(const _com_ptr_t<_OtherIID>& p) :
		::IHTMLDocument3Ptr(p)
	{
	}


	// Constructs a smart-pointer from any IUnknown-based interface pointer.
	//
	template<typename _InterfaceType> IHTMLDocument3Ptr(_InterfaceType* p) :
		::IHTMLDocument3Ptr(p)
	{
	}


	IHTMLElementCollectionPtr GetElementsByTagName(PCWSTR wzElementName)
	{
		IHTMLElementCollectionPtr rcollection;
		RaiseExceptionIfFailed(GetInterfacePtr()->getElementsByTagName(_bstr_t(wzElementName), &rcollection));
		return rcollection;
	}
};

}
