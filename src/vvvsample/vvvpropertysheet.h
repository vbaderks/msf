//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "propertypageitem.h"
#include <msf.h>

class VVVPropertySheet : public MSF::PropertySheet
{
public:
    VVVPropertySheet(const VVVPropertySheet&) = delete;
    VVVPropertySheet& operator=(const VVVPropertySheet&) = delete;

    VVVPropertySheet(VVVItem& item, IShellFolder* shellFolder) :
        PropertySheet(item.GetDisplayName(), PSH_NOAPPLYNOW | PSH_PROPTITLE | PSH_NOCONTEXTHELP)
    {
        AddPage(PropertyPageItem::CreateInstance(item, m_eventID, shellFolder));
    }

    int DoModal(HWND hwnd, long& wEventId)
    {
        m_eventID = 0;
        int result = __super::DoModal(hwnd);

        wEventId = m_eventID;
        return result;
    }

private:

    // Members variables
    long m_eventID{ 0 };
};
