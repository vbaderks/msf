//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "strutil.h"

namespace MSF
{


// Purpose: Simple class to create a propertysheet.
//          For more advanced functionality use WTL OR MFC.
//          There is extra support to return a mask that can be used
//          to reflect if the user made any actual changes.
//
class CPropertySheet
{
public:
    class CPropSheetHeader : public PROPSHEETHEADER
    {
    public:
        CPropSheetHeader(const CString& strCaption, DWORD dwflags = 0)
        {
            ZeroMemory(static_cast<PROPSHEETHEADER*>(this), sizeof(PROPSHEETHEADER));
            dwSize = PROPSHEETHEADER_V1_SIZE;
            dwFlags = dwflags;

            m_strCaption = strCaption;
            pszCaption = m_strCaption.GetString();
        }


        void SetPages(std::vector<HPROPSHEETPAGE>& hpages)
        {
            phpage = &hpages[0];
            nPages = static_cast<UINT>(hpages.size());
        }

    private:
        CString m_strCaption;
    };


    CPropertySheet(const CString& strCaption, DWORD dwFlags = 0)
        : _propsheetheader(strCaption, dwFlags), _wEventId(0)
    {
    }


    CPropertySheet(UINT nIDCaption, DWORD dwFlags = 0)
        : _propsheetheader(LoadString(nIDCaption), dwFlags), _wEventId(0)
    {
    }


    ~CPropertySheet()
    {
        // Clean up pages that are added but never used.
        for (auto hpage : _hpages)
        { 
            ATLVERIFY(DestroyPropertySheetPage(hpage));
        }
    }

    CPropertySheet(const CPropertySheet&) = delete;

    CPropertySheet& operator=(const CPropertySheet&) = delete;

    void AddPage(HPROPSHEETPAGE hpage)
    {
        _hpages.push_back(hpage);
    }


    int DoModal(HWND hwndParent)
    {
        _propsheetheader.hwndParent = hwndParent;
        _propsheetheader.SetPages(_hpages);

        int result = static_cast<int>(PropertySheet(&_propsheetheader));
        _hpages.clear();
        return result;
    }


    long DoModalReturnChanges(HWND hwndParent)
    {
        _wEventId = 0;
        int result = DoModal(hwndParent);
        if (result <= 0)
            return 0;

        return _wEventId;
    }

    long& GetEventId()
    {
        return _wEventId;
    }

private:

    // Member variables.
    CPropSheetHeader            _propsheetheader;
    std::vector<HPROPSHEETPAGE> _hpages;
    long                        _wEventId;
};

} // end MSF namespace.
