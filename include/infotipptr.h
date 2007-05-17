//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "queryinfoptr.h"
#include "persistfileptr.h"


namespace MSF
{

// An infotip has 2 main COM interfaces. Use IUnknown as the base interface to hold
// a reference on the general object.
class IInfoTipPtr : public IUnknownPtr
{
public:
	explicit IInfoTipPtr(const CLSID& clsid, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_INPROC_SERVER) :
		IUnknownPtr(clsid, pOuter, dwClsContext)
	{
	}


	void Load(PCWSTR wszFilename)
	{
		MSF::IPersistFilePtr persistfile(this);

		persistfile.Load(wszFilename);
	}


	CStringW GetInfoTip()
	{
		MSF::IQueryInfoPtr queryinfo(this);

		return queryinfo.GetInfoTip();
	}


	CStringW GetInfoTip(PCWSTR wszFilename)
	{
		Load(wszFilename);

		return GetInfoTip();
	}
};

} // end MSF namespace
