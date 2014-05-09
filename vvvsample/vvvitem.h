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


    static LPITEMIDLIST CreateItemIdList(unsigned int nId, unsigned int nSize, bool bFolder, const CString& strName)
    {
        LPITEMIDLIST pidl = CPidl::CreateItemIdListWithTerminator(sizeof(SItemData));

        InitializeItemData(reinterpret_cast<SItemData*>(pidl->mkid.abID),
            nId, nSize, bFolder, strName);

        return pidl;
    }


    explicit CVVVItem(LPCITEMIDLIST pidl) : CItemBase(pidl)
    {
        // Shell item IDs can be passed from external sources, validate
        // It is valid to pass a PIDL that is larger then the original (done by Search functionality in XP).
        bool bValid = GetDataSize() >= sizeof(SItemData) && GetItemData().nTypeID == TYPE_ID;
#ifdef _DEBUG
        if (!bValid)
        {
            ATLTRACE2(atlTraceCOM, 0, L"CVVVItem::Constructor, PIDL not valid (dsize=%d, ssize=%d)\n", GetDataSize(), sizeof(SItemData));
        }
#endif
        RaiseExceptionIf(!bValid);
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

    // By setting and checking for a TypeId (or cookie) we can ensure that the PIDL 
    // was created by us. Using a version # will allows to handle older persisted PIDLs
    const static unsigned int TYPE_ID = 0x5601; // 'V' + version #

    // By using a struct with a version # it becomes possible to detected old persisted PIDLs.
    #pragma pack(1) // By using an explicit pack the memory layout is better fixed then trusting the project settings.
    struct SItemData
    {
        unsigned short nTypeID;
        bool         bFolder;
        unsigned int nID;
        unsigned int nSize;
        wchar_t      wszName[MAX_PATH];
    };
    #pragma pack()

    static void InitializeItemData(SItemData* pitemdata, unsigned int nId, unsigned int nSize, bool bFolder, const CString& strName)
    {
        pitemdata->nTypeID = TYPE_ID;
        pitemdata->nID     = nId;
        pitemdata->bFolder = bFolder;
        pitemdata->nSize   = nSize;
        wcscpy_s(pitemdata->wszName, MSF_ARRAY_SIZE(pitemdata->wszName), CStringW(strName));
    }


    const SItemData& GetItemData() const throw()
    {
        return *reinterpret_cast<const SItemData*>(GetData());
    }

    int CompareByName(const CVVVItem& item) const;
};

typedef std::vector<CVVVItem> CVVVItemList;
