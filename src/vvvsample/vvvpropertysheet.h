//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#pragma once

#include "propertypageitem.h"
#include <msf.h>

class VVVPropertySheet : public msf::PropertySheet
{
public:
    VVVPropertySheet(VVVItem& item, IShellFolder* shellFolder) :
        PropertySheet(item.GetDisplayName(), PSH_NOAPPLYNOW | PSH_PROPTITLE | PSH_NOCONTEXTHELP)
    {
        AddPage(PropertyPageItem::CreateInstance(item, m_eventID, shellFolder));
    }

    ~VVVPropertySheet() = default;
    VVVPropertySheet(const VVVPropertySheet&) = delete;
    VVVPropertySheet(const VVVPropertySheet&&) = delete;
    VVVPropertySheet& operator=(const VVVPropertySheet&) = delete;
    VVVPropertySheet& operator=(VVVPropertySheet&& other) = delete;

    int DoModal(HWND hwnd, long& wEventId) noexcept
    {
        m_eventID = 0;
        const auto result = msf::PropertySheet::DoModal(hwnd);

        wEventId = m_eventID;
        return result;
    }

private:
    long m_eventID{};
};
