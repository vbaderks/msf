//
// (C) Copyright by Victor Derks
//
// See README.TXT for detailed details of the software licence.
//
#include "stdafx.h"
#include "vvvitem.h"
#include "columns.h"
#include "../include/macros.h"
#include "../include/strutil.h"
#include "../include/util.h"
#include "../include/imagelistindex.h"
#include "resource.h"


// Note: 'switch' is used in this example to show all the possible options.
CString CVVVItem::GetDisplayName(SHGDNF shgdnf) const
{
    switch (shgdnf)
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
        return ToString(GetID()); // return unique string (vvv items are unique by ID)

    default:
        ATLTRACE2(atlTraceCOM, 0, L"CVVVItem::GetDisplayName (shgdnf=%d)\n", shgdnf);
        break;
    }

    return GetName();
}


SFGAOF CVVVItem::GetAttributeOf(bool bSingleSelect, bool bReadOnly) const
{
    if (IsFolder())
    {
        return SFGAO_FOLDER | SFGAO_HASSUBFOLDER;
    }
    else
    {
        SFGAOF sfgaof = SFGAO_CANCOPY;

        // Tip: to debug error handling, disable the readonly check and make the .vvv readonly
        if (!bReadOnly)
        {
            sfgaof |= SFGAO_CANRENAME | SFGAO_CANDELETE | SFGAO_CANMOVE;

            // Note: standard windows UI allows the rename option for multiple items.
        }

        if (bSingleSelect)
        {
            sfgaof |= SFGAO_HASPROPSHEET; 
        }

        return sfgaof;
    }
}


int CVVVItem::Compare(const CVVVItem& item, int nCompareBy, bool /*bCanonicalOnly*/) const
{
    switch (nCompareBy)
    {
    case COLUMN_NAME:
        return CompareByName(item);

    case COLUMN_SIZE:
        return UIntCmp(GetSize(), item.GetSize());

    default:
        ATLASSERT(!"Illegal nCompare option detected");
        RaiseException();
    }
}


CString CVVVItem::GetItemDetailsOf(UINT iColumn) const
{
    switch (iColumn)
    {
    case COLUMN_NAME:
        return GetDisplayName(SHGDN_NORMAL);

    case COLUMN_SIZE:
        if (IsFolder())
            return CString();
        else
            return ToString(GetSize());

    default:
        ATLASSERT(false);
        RaiseException();
    }
}


CString CVVVItem::GetInfoTipText() const
{
    CString strText = LoadString(IDS_SHELLEXT_NAME) + L": " + GetDisplayName() + L"\n";
    strText += LoadString(IDS_SHELLEXT_SIZE) + L": " + ToString(GetSize());

    return strText;
}


int CVVVItem::GetIconOf(UINT flags) const
{
    if (IsFolder())
        return IsBitSet(flags, GIL_OPENICON) ?
            STD_IMAGELIST_INDEX_FOLDER_OPEN :
            STD_IMAGELIST_INDEX_FOLDER_PLAIN;

    return STD_IMAGELIST_INDEX_DOCUMENT_FILLED;
}


// Note: can only return 0 if same items are compared.
int CVVVItem::CompareByName(const CVVVItem& item) const
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
    return UIntCmp(GetID(), item.GetID());
}
