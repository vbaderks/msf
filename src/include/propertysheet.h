//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "strutil.h"

namespace MSF {

// Purpose: Simple class to create a property sheet.
//          For more advanced functionality use WTL OR MFC.
//          There is extra support to return a mask that can be used
//          to reflect if the user made any actual changes.
//
class PropertySheet
{
public:
    PropertySheet(const PropertySheet&) = delete;
    PropertySheet& operator=(const PropertySheet&) = delete;

    class PropSheetHeader : public PROPSHEETHEADER
    {
    public:
        explicit PropSheetHeader(std::wstring caption, DWORD dwflags = 0)
        {
            ZeroMemory(static_cast<PROPSHEETHEADER*>(this), sizeof(PROPSHEETHEADER));
            dwSize = PROPSHEETHEADER_V1_SIZE;
            dwFlags = dwflags;

            m_caption = caption;
            pszCaption = m_caption.c_str();
        }

        void SetPages(std::vector<HPROPSHEETPAGE>& hpages)
        {
            phpage = hpages.data();
            nPages = static_cast<UINT>(hpages.size());
        }

    private:
        std::wstring m_caption;
    };

    explicit PropertySheet(std::wstring strCaption, DWORD dwFlags = 0) :
        m_propertySheetHeader(strCaption, dwFlags),
        m_eventID(0)
    {
    }

    explicit PropertySheet(UINT nIDCaption, DWORD dwFlags = 0) :
        m_propertySheetHeader(LoadResourceString(nIDCaption), dwFlags),
        m_eventID(0)
    {
    }

    ~PropertySheet()
    {
        // Clean up pages that are added but never used.
        for (auto hpage : m_hpages)
        { 
            ATLVERIFY(DestroyPropertySheetPage(hpage));
        }
    }

    void AddPage(HPROPSHEETPAGE hpage)
    {
        m_hpages.push_back(hpage);
    }

    int DoModal(HWND hwndParent)
    {
        m_propertySheetHeader.hwndParent = hwndParent;
        m_propertySheetHeader.SetPages(m_hpages);

        auto result = static_cast<int>(::PropertySheet(&m_propertySheetHeader));
        m_hpages.clear();
        return result;
    }

    long DoModalReturnChanges(HWND hwndParent)
    {
        m_eventID = 0;
        int result = DoModal(hwndParent);
        if (result <= 0)
            return 0;

        return m_eventID;
    }

    long& GetEventId()
    {
        return m_eventID;
    }

private:

    // Member variables.
    PropSheetHeader             m_propertySheetHeader;
    std::vector<HPROPSHEETPAGE> m_hpages;
    long                        m_eventID;
};

} // end MSF namespace.
