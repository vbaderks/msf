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
class PropSheetHost
{
public:
    ~PropSheetHost()
    {
        for (auto hpropsheetpage : _propsheetpages)
        {
            ATLVERIFY(::DestroyPropertySheetPage(hpropsheetpage));
        }
    }

    void Add(HPROPSHEETPAGE hpropsheetpage)
    {
        _propsheetpages.push_back(hpropsheetpage);
    }

    static BOOL CALLBACK AddPage(HPROPSHEETPAGE hpropsheetpage, LPARAM lparam)
    {
        PropSheetHost* ppropsheethost = reinterpret_cast<PropSheetHost*>(lparam);
        ppropsheethost->Add(hpropsheetpage);
        return TRUE;
    }

private:

    std::vector<HPROPSHEETPAGE> _propsheetpages;
};

} // end msf namespace
