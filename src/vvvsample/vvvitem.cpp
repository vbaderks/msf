//
// (C) Copyright by Victor Derks
//
// See README.TXT for detailed details of the software license.
//

#include "stdafx.h"

#include "vvvitem.h"
#include "columns.h"
#include "resource.h"

using std::wstring;


// Note: 'switch' is used in this example to show all the possible options.
std::wstring VVVItem::GetDisplayName(SHGDNF shellGetDisplayNameType) const
{
    switch (shellGetDisplayNameType)
    {
    case SHGDN_NORMAL:
    case SHGDN_INFOLDER:
        break;

    case SHGDN_INFOLDER | SHGDN_FOREDITING:
        break; // override if item has a special edit name

    case SHGDN_INFOLDER | SHGDN_FORADDRESSBAR:
        break; // override if item has a special name for the address bar

    case SHGDN_INFOLDER | SHGDN_FORPARSING:
    case SHGDN_FORPARSING: // note parent should append folder name before item name.
        return std::to_wstring(GetID()); // return unique string (VVV items are unique by ID)

    default:
        ATLTRACE2(ATL::atlTraceCOM, 0, L"VVVItem::GetDisplayName (shellGetDisplayNameType=%d)\n", shellGetDisplayNameType);
        break;
    }

    return GetName();
}


SFGAOF VVVItem::GetAttributeOf(bool bSingleSelect, bool bReadOnly) const noexcept
{
    if (IsFolder())
    {
        return SFGAO_FOLDER | SFGAO_HASSUBFOLDER;
    }

    SFGAOF shellFileGetAttributesOfFlags = SFGAO_CANCOPY;

    // Tip: to debug error handling, disable the read only check and make the .vvv read only
    if (!bReadOnly)
    {
        shellFileGetAttributesOfFlags |= SFGAO_CANRENAME | SFGAO_CANDELETE | SFGAO_CANMOVE;

        // Note: standard windows UI allows the rename option for multiple items.
    }

    if (bSingleSelect)
    {
        shellFileGetAttributesOfFlags |= SFGAO_HASPROPSHEET;
    }

    return shellFileGetAttributesOfFlags;
}


int VVVItem::Compare(const VVVItem& item, int compareBy, bool /*bCanonicalOnly*/) const
{
    switch (compareBy)
    {
    case ColumnName:
        return CompareByName(item);

    case ColumnSize:
        return msf::UIntCmp(GetSize(), item.GetSize());

    default:
        ATLASSERT(!"Illegal nCompare option detected");
        msf::RaiseException();
    }
}


wstring VVVItem::GetItemDetailsOf(UINT columnIndex) const
{
    switch (columnIndex)
    {
    case ColumnName:
        return GetDisplayName(SHGDN_NORMAL);

    case ColumnSize:
        {
            if (IsFolder())
                return wstring();

            return std::to_wstring(GetSize());
        }

    default:
        ATLASSERT(false);
        msf::RaiseException();
    }
}


std::wstring VVVItem::GetInfoTipText() const
{
    return msf::LoadResourceString(IDS_SHELLEXT_NAME) + L": " + wstring(GetDisplayName()) + L"\n" +
           msf::LoadResourceString(IDS_SHELLEXT_SIZE) + L": " + std::to_wstring(GetSize());
}


int VVVItem::GetIconOf(UINT flags) const noexcept
{
    if (IsFolder())
        return msf::IsBitSet(flags, GIL_OPENICON) ?
        static_cast<int>(msf::StandardImagelistIndex::FolderOpen) :
        static_cast<int>(msf::StandardImagelistIndex::FolderPlain);

    return static_cast<int>(msf::StandardImagelistIndex::DocumentFilled);
}


// Note: can only return 0 if same items are compared.
int VVVItem::CompareByName(const VVVItem& item) const noexcept
{
    // Compare first by folder, to make sure folders are listed before files.
    if (IsFolder())
    {
        if (!item.IsFolder())
            return -1;
    }
    else
    {
        if (item.IsFolder())
            return 1;
    }

    const auto nResult = StrCmpW(GetItemData().wszName, item.GetItemData().wszName);
    if (nResult != 0)
        return nResult; // different by name

    // VVV items can be equal by name, but are always different by ID.
    return msf::UIntCmp(GetID(), item.GetID());
}
