//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "cfeffect.h"


namespace MSF
{

class CCfTargetCLSID
{
public:

	static void Get(IDataObject* pdataobject, CLSID& clsid)
	{
		RaiseExceptionIfFailed(GetImpl(pdataobject, clsid));
	}


	static bool GetOptional(IDataObject* pdataobject, CLSID& clsid)
	{
		HRESULT hr = GetImpl(pdataobject, clsid);
		return SUCCEEDED(hr);
	}

private:

	HRESULT GetImpl(IDataObject* pdataobject, CLSID& clsid)
	{
		CFormatEtc formatetc(CFSTR_TARGETCLSID);
		CStgMedium medium;
		HRESULT hr = pdataobject->GetData(&formatetc, &medium);
		if (FAILED(hr))
			return hr;

		CGlobalLock<CLSID> globallock(medium.hGlobal);

		clsid = *globallock.get();
		return S_OK;
	}
};

} // end of MSF namespace
