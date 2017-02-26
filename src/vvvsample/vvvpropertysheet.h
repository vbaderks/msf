//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "../include/propertysheet.h"
#include "propertypageitem.h"


class CVVVPropertySheet : public CPropertySheet
{
public:
    CVVVPropertySheet(VVVItem& item, IShellFolder* pshellfolder) :
        CPropertySheet(item.GetDisplayName(), PSH_NOAPPLYNOW | PSH_PROPTITLE | PSH_NOCONTEXTHELP),
        _wEventId(0)
    {
        AddPage(CPropertyPageItem::CreateInstance(item, _wEventId, pshellfolder));
    }

    CVVVPropertySheet(const CVVVPropertySheet&) = delete;
    CVVVPropertySheet& operator=(const CVVVPropertySheet&) = delete;

    int DoModal(HWND hwnd, long& wEventId)
    {
        _wEventId = 0;
        int result = __super::DoModal(hwnd);

        wEventId = _wEventId;
        return result;
    }

private:

    // Members variables
    long _wEventId;
};
