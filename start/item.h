//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for detailed details of the software licence.
//
#pragma once


#include <string.h>
#include <vector>
#include "../include/macros.h"
#include "../include/pidl.h"
#include "../include/itembase.h"


class CItem : public CItemBase
{
public:
	static ITEMIDLIST* CreateItemIdList(/* TODO: pass arguments */)
	{
		ITEMIDLIST* pidl = CPidl::CreateItemIdListWithTerminator(sizeof(SItemData));

		// TODO: initialize members
	
		return pidl;	
	}


	explicit CItem(const ITEMIDLIST* pidl) : CItemBase(pidl)
	{
		// Shell item IDs can be passed from external sources, validate
		// TODO: xxx
	}

	
	CString GetDisplayName(SHGDNF /*shgdnf*/) const
	{
		return CString();
	}

	
	int GetIconOf(UINT /*flags*/) const
	{
		return 0;
	}


	int Compare(const CItem& /*directoryitem*/, int /*nCompareBy*/, bool /*bCanonicalOnly*/) const
	{
		return 0;
	}


	CString GetInfoTipText() const
	{
		return CString();
	}


	CString GetItemDetailsOf(UINT /*iColumn*/) const
	{
		return CString();
	}


private:

	struct SItemData
	{
		// TODO: add members
	};
};

typedef std::vector<CItem> CItems;
