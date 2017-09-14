//
// (C) Copyright by Victor Derks
//
// See README.TXT for detailed details of the software licence.
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
        return MSF::ToString(GetID()); // return unique string (VVV items are unique by ID)

    default:
        ATLTRACE2(ATL::atlTraceCOM, 0, L"VVVItem::GetDisplayName (shellGetDisplayNameType=%d)\n", shellGetDisplayNameType);
        break;
    }

    return GetName();
}


SFGAOF VVVItem::GetAttributeOf(bool bSingleSelect, bool bReadOnly) const
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
    case COLUMN_NAME:
        return CompareByName(item);

    case COLUMN_SIZE:
        return MSF::UIntCmp(GetSize(), item.GetSize());

    default:
        ATLASSERT(!"Illegal nCompare option detected");
        MSF::RaiseException();
    }
}


wstring VVVItem::GetItemDetailsOf(UINT columnIndex) const
{
    switch (columnIndex)
    {
    case COLUMN_NAME:
        return GetDisplayName(SHGDN_NORMAL);

    case COLUMN_SIZE:
        {
            if (IsFolder())
                return wstring();

            return MSF::ToString(GetSize());
        }

    default:
        ATLASSERT(false);
        MSF::RaiseException();
    }
}


std::wstring VVVItem::GetInfoTipText() const
{
    return MSF::LoadResourceString(IDS_SHELLEXT_NAME) + L": " + wstring(GetDisplayName()) + L"\n" +
           MSF::LoadResourceString(IDS_SHELLEXT_SIZE) + L": " + wstring(MSF::ToString(GetSize()));
}


int VVVItem::GetIconOf(UINT flags) const
{
    if (IsFolder())
        return MSF::IsBitSet(flags, GIL_OPENICON) ?
        static_cast<int>(MSF::StandardImagelistIndex::FolderOpen) :
        static_cast<int>(MSF::StandardImagelistIndex::FolderPlain);

    return static_cast<int>(MSF::StandardImagelistIndex::DocumentFilled);
}


// Note: can only return 0 if same items are compared.
int VVVItem::CompareByName(const VVVItem& item) const
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

    int nResult = StrCmpW(GetItemData().wszName, GetItemData().wszName);
    if (nResult != 0)
        return nResult; // different by name

    // VVV items can be equal by name, but are always different by ID.
    return MSF::UIntCmp(GetID(), item.GetID());
}
