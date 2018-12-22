//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include <vector>

#pragma comment(lib, "comctl32") // required to link DestroyPropertySheetPage.

namespace msf
{

/// <purpose>Owns a collection of property sheet pages.</purpose>
class PropSheetHost final
{
public:
    ~PropSheetHost()
    {
        for (auto propSheetPage : m_propSheetPages)
        {
            ATLVERIFY(::DestroyPropertySheetPage(propSheetPage));
        }
    }

    void Add(HPROPSHEETPAGE propSheetPage)
    {
        m_propSheetPages.push_back(propSheetPage);
    }

    static BOOL CALLBACK AddPage(HPROPSHEETPAGE propSheetPage, LPARAM lparam)
    {
        PropSheetHost* host = reinterpret_cast<PropSheetHost*>(lparam);
        host->Add(propSheetPage);
        return true;
    }

private:

    std::vector<HPROPSHEETPAGE> m_propSheetPages;
};

} // end msf namespace
