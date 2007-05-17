//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include <objidl.h>
#include "util.h"
#include "globallock.h"


namespace MSF
{

// A CStgMedium class is owner of the stgmedium. 

class CStgMedium : public STGMEDIUM
{
public:
	static HGLOBAL GlobalClone(HGLOBAL hglobIn)
	{
		if (hglobIn == NULL)
			return NULL;

		CGlobalLock<void> globallock(hglobIn);

		size_t cb = GlobalSize(hglobIn);
		HGLOBAL hglobOut = GlobalAllocThrow(cb);
		CopyMemory(hglobOut, globallock.get(), cb);
		return hglobOut;
	}


	static void SetHGlobal(STGMEDIUM& stgmedium, HGLOBAL hglobal) throw()
	{
		stgmedium.tymed          = TYMED_HGLOBAL;
		stgmedium.hGlobal        = hglobal;
		stgmedium.pUnkForRelease = NULL;
	}


	CStgMedium() throw()
	{
		tymed = TYMED_NULL;
		pUnkForRelease = NULL;
	}


	CStgMedium(HGLOBAL hg) throw()
	{
		SetHGlobal(*this, hg);
	}


	// Purpose: passed in STGMEDIUM will be owned after a bitwise copy.
	CStgMedium(const STGMEDIUM& stgmedium) throw()
	{
		*static_cast<STGMEDIUM*>(this) = stgmedium;
	}


	~CStgMedium() throw()
	{
		if (tymed != TYMED_NULL)
		{
			ReleaseStgMedium(this);
		}
	}


	HGLOBAL GetHGlobal() const throw()
	{
		ATLASSERT(tymed == TYMED_HGLOBAL && "Can only get hglobal if correct type");
		return hGlobal;
	}


	void Detach(STGMEDIUM& stgmedium) throw()
	{
		stgmedium = *this;

		Detach();
	}


	void Detach() throw()
	{
		tymed = TYMED_NULL;
	}


	CStgMedium& operator = (STGMEDIUM& stgmedium)
	{
		Detach();

		*static_cast<STGMEDIUM*>(this) = stgmedium;
		
		stgmedium.tymed = TYMED_NULL;
		stgmedium.pUnkForRelease = NULL;

		return *this;
	}


	void CopyTo(STGMEDIUM& stgmedium)
	{
		stgmedium.tymed          = tymed;
		stgmedium.pUnkForRelease = pUnkForRelease;

		switch (tymed)
		{
		case TYMED_HGLOBAL:
			stgmedium.hGlobal = GlobalClone(GetHGlobal());
			break;

		case TYMED_ISTREAM:
			stgmedium.pstm = pstm;
			stgmedium.pstm->AddRef();
			break;

		case TYMED_ISTORAGE:
			stgmedium.pstg = pstg;
			stgmedium.pstg->AddRef();
			break;
		
		default:
			ATLASSERT(!"other types are currently not supported.");
			break;
		}
	}
};

} // end MSF namespace
