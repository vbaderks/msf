//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "strutil.h"

namespace msf {

// Purpose: Simple class to create a property sheet.
//          For more advanced functionality use WTL OR MFC.
//          There is extra support to return a mask that can be used
//          to reflect if the user made any actual changes.
//
class PropertySheet
{
public:
    PropertySheet(const PropertySheet&) = delete;
    PropertySheet(PropertySheet&&) = delete;
    PropertySheet& operator=(const PropertySheet&) = delete;
    PropertySheet& operator=(PropertySheet&&) = delete;

    class PropSheetHeader : public PROPSHEETHEADER
    {
    public:
        explicit PropSheetHeader(std::wstring caption, DWORD dwflags = 0) :
            m_caption{std::move(caption)}
        {
            ZeroMemory(static_cast<PROPSHEETHEADER*>(this), sizeof(PROPSHEETHEADER));
            dwSize = PROPSHEETHEADER_V1_SIZE;
            dwFlags = dwflags;

            pszCaption = m_caption.c_str();
        }

        void SetPages(std::vector<HPROPSHEETPAGE>& hpages) noexcept
        {
            phpage = hpages.data();
            nPages = static_cast<UINT>(hpages.size());
        }

    private:
        std::wstring m_caption;
    };

    explicit PropertySheet(std::wstring strCaption, DWORD dwFlags = 0) :
        m_propertySheetHeader(strCaption, dwFlags)
    {
    }

    explicit PropertySheet(UINT nIDCaption, DWORD dwFlags = 0) :
        m_propertySheetHeader(LoadResourceString(nIDCaption), dwFlags)
    {
    }

    virtual ~PropertySheet()
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

    int DoModal(HWND hwndParent) noexcept
    {
        m_propertySheetHeader.hwndParent = hwndParent;
        m_propertySheetHeader.SetPages(m_hpages);

        const auto result = static_cast<int>(::PropertySheet(&m_propertySheetHeader));
        m_hpages.clear();
        return result;
    }

    long DoModalReturnChanges(HWND hwndParent) noexcept
    {
        m_eventID = 0;
        const auto result = DoModal(hwndParent);
        if (result <= 0)
            return 0;

        return m_eventID;
    }

    long& GetEventId() noexcept
    {
        return m_eventID;
    }

private:
    PropSheetHeader             m_propertySheetHeader;
    std::vector<HPROPSHEETPAGE> m_hpages;
    long                        m_eventID{};
};

} // end msf namespace.
