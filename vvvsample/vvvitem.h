//
// (C) Copyright by Victor Derks
//
// See README.TXT for detailed details of the software licence.
//
#pragma once


#include <string.h>
#include <vector>
#include "../include/macros.h"
#include "../include/pidl.h"
#include "../include/itembase.h"


class CVVVItem : public CItemBase
{
public:
	static int GetMaxNameLength(LPCWSTR /*pszName*/) throw()
	{
		return 10; // note: limit is 10 for easy testing.
	}


	static ITEMIDLIST* CreateItemIdList(unsigned int nId, unsigned int nSize, bool bFolder, const CString& strName)
	{
		ITEMIDLIST* pidl = CPidl::CreateItemIdListWithTerminator(sizeof(SItemData));

		InitializeItemData(reinterpret_cast<SItemData*>(pidl->mkid.abID),
			nId, nSize, bFolder, strName);
	
		return pidl;	
	}


	explicit CVVVItem(const ITEMIDLIST* pidl) : CItemBase(pidl)
	{
		// Shell item IDs can be passed from external sources, validate
		RaiseExceptionIf(GetDataSize() != sizeof(SItemData));
	}


	CString GetDisplayName(SHGDNF shgdnf = SHGDN_NORMAL) const;

	SFGAOF GetAttributeOf(bool bSingleSelect, bool bReadOnly) const;

	unsigned int GetID() const throw()
	{
		return GetItemData().nID;
	}


	unsigned int GetSize() const throw()
	{
		return GetItemData().nSize;
	}


	CString GetName() const
	{
		return CString(CW2CT(GetItemData().wszName));
	}


	bool IsFolder() const throw()
	{
		return GetItemData().bFolder;
	}


	int Compare(const CVVVItem& directoryitem, int nCompareBy, bool bCanonicalOnly) const;
	CString GetItemDetailsOf(UINT iColumn) const;
	CString GetInfoTipText() const;
	int GetIconOf(UINT flags) const; 

private:

	struct SItemData
	{
		unsigned int nID;
		unsigned int nSize;
		bool         bFolder;
		wchar_t      wszName[MAX_PATH];
	};


	static void InitializeItemData(SItemData* pitemdata, unsigned int nId, unsigned int nSize, bool bFolder, const CString& strName)
	{
		pitemdata->nID     = nId;
		pitemdata->bFolder = bFolder;
		pitemdata->nSize   = nSize;
		wcscpy(pitemdata->wszName, CStringW(strName));
	}


	const SItemData& GetItemData() const throw()
	{
		return *reinterpret_cast<const SItemData*>(GetData());
	}

	int CompareByName(const CVVVItem& item) const;
};

typedef std::vector<CVVVItem> CVVVItemList;
