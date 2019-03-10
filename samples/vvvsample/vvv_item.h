//
// (C) Copyright by Victor Derks
//
// See README.TXT for detailed details of the software license.
//
#pragma once


#include <msf.h>


class VVVItem : public msf::ItemBase
{
public:
    static int GetMaxNameLength(PCWSTR /*pszName*/) noexcept
    {
        return 10; // note: limit is 10 for easy testing.
    }

    static PUIDLIST_RELATIVE CreateItemIdList(unsigned int id, unsigned int size, bool folder, const std::wstring& name)
    {
        const PUIDLIST_RELATIVE pidl = msf::ItemIDList::CreateItemIdListWithTerminator(sizeof(SItemData));

        InitializeItemData(reinterpret_cast<SItemData*>(pidl->mkid.abID),
            id, size, folder, name);

        return pidl;
    }

    explicit VVVItem(PCUIDLIST_RELATIVE pidl) : ItemBase(pidl)
    {
        // Shell item IDs can be passed from external sources, validate
        // It is valid to pass a PIDL that is larger then the original (done by Search functionality in XP).
        const bool valid = GetDataSize() >= sizeof(SItemData) && GetItemData().nTypeID == TypeID;
#ifdef _DEBUG
        if (!valid)
        {
            ATLTRACE(L"VVVItem::Constructor, PIDL not valid (data_size=%d, s_item_size=%d)\n", GetDataSize(), sizeof(SItemData));
        }
#endif
        msf::RaiseExceptionIf(!valid);
    }

    std::wstring GetDisplayName(SHGDNF shellGetDisplayNameType = SHGDN_NORMAL) const;

    SFGAOF GetAttributeOf(bool bSingleSelect, bool bReadOnly) const noexcept;

    unsigned int GetID() const noexcept
    {
        return GetItemData().id;
    }

    unsigned int GetSize() const noexcept
    {
        return GetItemData().size;
    }

    std::wstring GetName() const
    {
        return GetItemData().name;
    }

    bool IsFolder() const noexcept
    {
        return GetItemData().folder;
    }

    int Compare(const VVVItem& item, int compareBy, bool bCanonicalOnly) const noexcept;
    std::wstring GetItemDetailsOf(uint32_t columnIndex) const;
    std::wstring GetInfoTipText() const;
    int GetIconOf(uint32_t flags) const noexcept;

private:

    // By setting and checking for a TypeId (or cookie) we can ensure that the PIDL
    // was created by us. Using a version # will allows to handle older persisted PIDLs
    constexpr static unsigned int TypeID = 0x5601; // 'V' + version #

    // By using a struct with a version # it becomes possible to detected old persisted PIDLs.
    #pragma pack(1) // By using an explicit pack the memory layout is better fixed then trusting the project settings.
    struct SItemData
    {
        unsigned short nTypeID;
        bool         folder;
        unsigned int id;
        unsigned int size;
        wchar_t      name[MAX_PATH];
    };
    #pragma pack()

    static void InitializeItemData(SItemData* itemData, unsigned int id, unsigned int size, bool folder, const std::wstring& name) noexcept
    {
        itemData->nTypeID = TypeID;
        itemData->id     = id;
        itemData->folder = folder;
        itemData->size   = size;
        wcscpy_s(itemData->name, _countof(itemData->name), name.c_str());
    }

    const SItemData& GetItemData() const noexcept
    {
        return *static_cast<const SItemData*>(GetData());
    }

    int CompareByName(const VVVItem& item) const noexcept;
};
