//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


namespace MSF
{

class IWebBrowser2Ptr : public ::IWebBrowser2Ptr
{
public:
	// Constructs a smart-pointer from any other smart pointer.
	//
	template<typename _OtherIID> IWebBrowser2Ptr(const _com_ptr_t<_OtherIID>& p) :
		::IWebBrowser2Ptr(p)
	{
	}


	// Constructs a smart-pointer from any IUnknown-based interface pointer.
	//
	template<typename _InterfaceType> IWebBrowser2Ptr(_InterfaceType* p) :
		::IWebBrowser2Ptr(p)
	{
	}


	IDispatchPtr GetDocument()
	{
		IDispatchPtr rdispatch;

		RaiseExceptionIfFailed(GetInterfacePtr()->get_Document(&rdispatch));
		return rdispatch;
	}
};

}
