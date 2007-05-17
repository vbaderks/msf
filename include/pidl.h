//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace MSF
{

// Purpose: Management class for pidls. A CPidl class is owner of the wrapped ITEMIDLIST.
class CPidl
{
public:
	static ITEMIDLIST* Clone(const ITEMIDLIST* pidlSrc)
	{
		ATLASSERT(pidlSrc != NULL && "Why clone a NULL pointer?");

		ITEMIDLIST* pidl = ILClone(pidlSrc);
		RaiseExceptionIf(pidl == NULL, E_OUTOFMEMORY);

		return pidl;
	}


	static ITEMIDLIST* Combine(const ITEMIDLIST* pidl1, const ITEMIDLIST* pidl2)
	{
		ITEMIDLIST* pidl = ILCombine(pidl1, pidl2);
		RaiseExceptionIf(pidl == NULL && !(pidl1 == NULL && pidl == NULL), E_OUTOFMEMORY);

		return pidl;
	}


	static ITEMIDLIST* CreateFromPath(LPCTSTR pszPath)
	{
		ITEMIDLIST* pidl = ILCreateFromPath(pszPath);
		RaiseExceptionIf(pidl == NULL, E_OUTOFMEMORY);

		return pidl;
	}


	static ITEMIDLIST* CreateItemIdListWithTerminator(size_t sizeItem)
	{
		size_t size = sizeof(short) + sizeItem;

		ITEMIDLIST* pidl = static_cast<ITEMIDLIST*>(CoTaskMemAlloc(size + (sizeof(short))));
		if (pidl == NULL)
			RaiseException(E_OUTOFMEMORY);

		SHITEMID* pshitemid = &(pidl->mkid);
		pshitemid->cb = static_cast<USHORT>(size);

		ITEMIDLIST* pidlTerminator = ILGetNext(pidl);
		SHITEMID* pItemIdTerminator = &(pidlTerminator->mkid);
		pItemIdTerminator->cb = 0;

		return pidl;
	}


	// Purpose: Small helper, returns NULL also for the tail element.
	static const ITEMIDLIST* GetNextItem(const ITEMIDLIST* pidl)
	{
		const ITEMIDLIST* pidlNext = ILGetNext(pidl);
		if (pidlNext != NULL && pidlNext->mkid.cb == 0)
		{
			pidlNext = NULL;
		}

		return pidlNext;
	}


	CPidl() throw() : m_pidl(NULL)
	{
	}


	// Purpose: special constructor for NULL pointer init.
	CPidl(int null) throw() : m_pidl(NULL)
	{
		(null);
		ATLASSERT(null == 0 && "Detected misuse of the special constructor");
	}


	CPidl(ITEMIDLIST* pidl) throw() : m_pidl(pidl)
	{
	}


	CPidl(const ITEMIDLIST* pidl1, const ITEMIDLIST* pidl2) :
		m_pidl(Combine(pidl1, pidl2))
	{
	}


	CPidl(const CPidl& pidl1, const ITEMIDLIST* pidl2) :
		m_pidl(Combine(pidl1.m_pidl, pidl2))
	{
	}


	CPidl(const TCHAR* pszPath) :
		m_pidl(CreateFromPath(pszPath))
	{
	}


	~CPidl() throw()
	{
		ILFree(m_pidl);
	}


	void Attach(ITEMIDLIST* pidl) throw()
	{
		ILFree(m_pidl);
		m_pidl = pidl;
	}


	ITEMIDLIST* Detach() throw()
	{
		ITEMIDLIST* pidl = m_pidl;
		m_pidl = NULL;
		return pidl;
	}


	void CloneFrom(const ITEMIDLIST* pidl)
	{
		Attach(Clone(pidl));
	}


	ITEMIDLIST* Clone() const
	{
		return Clone(m_pidl);
	}


	void AppendID(const SHITEMID* pmkid) throw()
	{
		ITEMIDLIST* pidl = ILAppendID(m_pidl, pmkid, TRUE);
		RaiseExceptionIf(pidl == NULL, E_OUTOFMEMORY);

		m_pidl = pidl;
	}


	const ITEMIDLIST* get() const throw()
	{
		return m_pidl;
	}


	operator const ITEMIDLIST*() const throw()
	{
		return get();
	}


	const ITEMIDLIST* operator->() const throw()
	{
		return get();
	}


	UINT GetSize() const throw()
	{
		return ILGetSize(m_pidl);
	}


	// Purpose: Adres operator to be used for passing address to be used as an out-parameter.
	ITEMIDLIST** operator&() throw()
	{
		Attach(NULL);
		return &m_pidl;
	}

private:

	ITEMIDLIST* m_pidl;
};

} // namespace MSF
