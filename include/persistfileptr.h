//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace MSF
{

class IPersistFilePtr : public ::IPersistFilePtr
{
public:
	explicit IPersistFilePtr(const CLSID& clsid, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_INPROC_SERVER) :
		::IPersistFilePtr(clsid, pOuter, dwClsContext)
	{
	}


	// Purpose: Constructs a smart-pointer from any other smart pointer.
	template<typename _OtherIID> IPersistFilePtr(const _com_ptr_t<_OtherIID>& p) :
		::IPersistFilePtr(p)
	{
	}


	// Purpose: Constructs a smart-pointer from any IUnknown-based interface pointer.
	template<typename _InterfaceType> IPersistFilePtr(_InterfaceType* p) :
		::IPersistFilePtr(p)
	{
	}


	void Load(PCWSTR wszFilename, DWORD dwMode = 0)
	{
		RaiseExceptionIfFailed(GetInterfacePtr()->Load(wszFilename, dwMode));
	}
};

} // end MSF namespace
