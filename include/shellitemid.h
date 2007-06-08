//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace MSF
{

struct ShellItemIdMember
{
	size_t      size;
	const void* pdata;
};


class CShellItemIdMember : public ShellItemIdMember
{
public:
	CShellItemIdMember(const bool* pbValue)
	{
		size = sizeof(*pbValue);
		pdata = pbValue;
	}


	CShellItemIdMember(const unsigned int* pnValue)
	{
		size = sizeof(*pnValue);
		pdata = pnValue;
	}


	CShellItemIdMember(PCWSTR wszValue)
	{
		size = (wcslen(wszValue) + 1) * sizeof(wchar_t);
		pdata = wszValue;
	}
};


class CShellItemId
{
public:
	CShellItemId(const ShellItemIdMember* members, unsigned int count, bool bAddEmptyId = true) throw()
	{
		size_t size = sizeof(short) + GetMembersSize(members, count);

		_pItemId = static_cast<SHITEMID*>(CoTaskMemAlloc(size + (bAddEmptyId ? sizeof(short) : 0)));
		if (_pItemId == NULL)
			return;

		SetSize(size);

		// Copy members into allocated SHITEMID.
		BYTE* p = _pItemId->abID;
		for (unsigned int i = 0; i < count; ++i)
		{
			memcpy(p, members[i].pdata, members[i].size);
			p += members[i].size;
		}

		// Add optional terminating SHITEMID to make it easy to convert to a ITEMIDLIST.
		if (bAddEmptyId)
		{
			SHITEMID* pItemIdTerminating = reinterpret_cast<SHITEMID*>(p);
			pItemIdTerminating->cb = 0;
		}
	}


	~CShellItemId() throw()
	{
		CoTaskMemFree(_pItemId);
	}


	SHITEMID* Detach() throw()
	{
		SHITEMID* pItemId = _pItemId;
		_pItemId = NULL;
		return pItemId;
	}

private:

	void SetSize(size_t size) throw()
	{
		ATLASSERT(size <= USHRT_MAX && "size will be sliced!");
		_pItemId->cb = static_cast<USHORT>(size);
	}


	static size_t GetMembersSize(const ShellItemIdMember* members, unsigned int count) throw()
	{
		size_t size = 0;

		for (unsigned int i = 0; i < count; ++i)
		{
			size += members[i].size;
		}

		return size;
	}


	SHITEMID* _pItemId;
};


class CShellItemIterator
{
public:
	CShellItemIterator(const SHITEMID& itemid) throw() : _p(itemid.abID)
	{
#ifdef _DEBUG
		_pitemid = &itemid;
#endif
	}

	~CShellItemIterator() throw()
	{
#ifdef _DEBUG
		size_t s = static_cast<size_t>(_p - _pitemid->abID);
		ATLASSERT(s == (_pitemid->cb - sizeof(unsigned short)) && "not all items were retrieved!");
#endif
	}


	bool GetBool() throw()
	{
		bool b = *reinterpret_cast<const bool *>(_p);
		_p += sizeof(b);
		return b;
	}


	unsigned int GetUnsignedInt() throw()
	{
		unsigned int n = *reinterpret_cast<const unsigned int *>(_p);
		_p += sizeof(n);
		return n;
	}


	CString GetString()
	{
		// Note: strings are always stored in Unicode to prevent ANSI/Unicode mismatches.
		CStringW strW(reinterpret_cast<PCWSTR>(_p));
		_p += (strW.GetLength() + 1) * sizeof(wchar_t);

		CString str = CW2T(strW);
		return str;
	}

private:
	const BYTE* _p;

#ifdef _DEBUG
	const SHITEMID* _pitemid;
#endif
};

} // namespace MSF
