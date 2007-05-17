//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "formatetc.h"
#include "stgmedium.h"
#include "util.h"


namespace MSF
{

inline HRESULT SetCfEffect(LPCTSTR lpszFormat, IDataObject* pdataobject, DWORD dwEffect)
{
	CStgMedium stgmedium(GlobalAllocThrow(sizeof(DWORD)));

	*static_cast<DWORD*>(stgmedium.GetHGlobal()) = dwEffect;

	// Note: the shell expects fRelease to be true.
	CFormatEtc formatetc(RegisterCf(lpszFormat));
	HRESULT hr = pdataobject->SetData(&formatetc, &stgmedium, true);
	if (SUCCEEDED(hr))
	{
		stgmedium.Detach();
	}

	return hr;
}


inline HRESULT GetCfEffect(LPCTSTR lpszFormat, IDataObject* pdataobject, DWORD& dwEffect)
{
	CFormatEtc formatetc(RegisterCf(lpszFormat));
	CStgMedium medium;
	HRESULT hr = pdataobject->GetData(&formatetc, &medium);
	if (FAILED(hr))
		return hr;

	CGlobalLock<DWORD> globallock(medium.hGlobal);

	dwEffect = *globallock.get();
	return S_OK;
}

} // end of MSF namespace
