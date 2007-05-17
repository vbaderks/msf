//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "util.h"


namespace MSF
{

// Purpose: base class for Clipboard format handlers.
class CCfHandler
{
public:
	CCfHandler(CLIPFORMAT clipformat, bool bCanGetData, bool bCanSetData) :
		_clipformat(clipformat),
		_bCanGetData(bCanGetData),
		_bCanSetData(bCanSetData)
	{
	}


	CCfHandler(LPCTSTR lpszFormat, bool bCanGetData, bool bCanSetData) :
		_clipformat(RegisterCf(lpszFormat)),
		_bCanGetData(bCanGetData),
		_bCanSetData(bCanSetData)
	{
	}


	virtual ~CCfHandler() throw()
	{
	}


	CLIPFORMAT GetClipFormat() const throw()
	{
		return _clipformat;
	}


	bool CanGetData() const throw()
	{
		return _bCanGetData;
	}


	bool CanSetData() const throw()
	{
		return _bCanSetData;
	}


	virtual HRESULT Validate(const FORMATETC& formatetc) const throw()
	{
		if (formatetc.dwAspect != DVASPECT_CONTENT)
			return DV_E_DVASPECT;

		if (!IsBitSet(formatetc.tymed, TYMED_HGLOBAL))
			return DV_E_TYMED;

		if (formatetc.lindex != -1)
			return DV_E_LINDEX;

		return S_OK;
	}


	virtual void GetData(const FORMATETC&, STGMEDIUM&) const
	{
	}


	virtual void SetData(const FORMATETC&, STGMEDIUM&, bool /*bRelease*/)
	{
	}


	bool IsValid(const FORMATETC& formatetc, const STGMEDIUM& stgmedium) const
	{
		return SUCCEEDED(Validate(formatetc)) && formatetc.tymed == stgmedium.tymed;
	}

private:

	// Member variables.
	CLIPFORMAT _clipformat;
	bool       _bCanGetData;
	bool       _bCanSetData;
};

}
