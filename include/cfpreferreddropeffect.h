//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "cfeffect.h"


namespace MSF
{

class CCfPreferredDropEffect
{
public:

	static DWORD Get(IDataObject* pdataobject)
	{
		DWORD dwEffect;
		RaiseExceptionIfFailed(GetCfEffect(CFSTR_PREFERREDDROPEFFECT, pdataobject, dwEffect));

		return dwEffect;
	}


	static DWORD GetOptional(IDataObject* pdataobject, DWORD dwEffectDefault = DROPEFFECT_NONE)
	{
		DWORD dwEffect;

		HRESULT hr = GetCfEffect(CFSTR_PREFERREDDROPEFFECT, pdataobject, dwEffect);
		if (SUCCEEDED(hr))
			return dwEffect;

		ATLTRACE2(atlTraceCOM, 0, _T("CCfPreferredDropEffect::GetOptional failed, hr=%x\n"), hr);
		return dwEffectDefault;
	}


	static void Set(IDataObject* pdataobject, DWORD dwEffect)
	{
		RaiseExceptionIfFailed(SetCfEffect(CFSTR_PREFERREDDROPEFFECT, pdataobject, dwEffect));
	}
};

} // end of MSF namespace
