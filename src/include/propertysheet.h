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
        explicit PropSheetHeader(std::wstring caption, DWORD flags = 0) :
            PROPSHEETHEADER{sizeof(PROPSHEETHEADER), flags},
            m_caption{std::move(caption)}
        {
            pszCaption = m_caption.c_str();
        }

        void SetPages(std::vector<HPROPSHEETPAGE>& pages) noexcept
        {
            phpage = pages.data();
            nPages = static_cast<UINT>(pages.size());
        }

    private:
        std::wstring m_caption;
    };

    explicit PropertySheet(std::wstring caption, DWORD flags = 0) :
        m_propertySheetHeader(std::move(caption), flags)
    {
    }

    explicit PropertySheet(UINT captionId, DWORD flags = 0) :
        m_propertySheetHeader(LoadResourceString(captionId), flags)
    {
    }

    virtual ~PropertySheet()
    {
        // Clean up pages that are added but never used.
        for (auto page : m_pages)
        {
            ATLVERIFY(DestroyPropertySheetPage(page));
        }
    }

    void AddPage(HPROPSHEETPAGE page)
    {
        m_pages.push_back(page);
    }

    int DoModal(HWND parentWindow) noexcept
    {
        m_propertySheetHeader.hwndParent = parentWindow;
        m_propertySheetHeader.SetPages(m_pages);

        const auto result = static_cast<int>(::PropertySheet(&m_propertySheetHeader));
        m_pages.clear();
        return result;
    }

    long DoModalReturnChanges(HWND parentWindow) noexcept
    {
        m_eventId = 0;
        const auto result = DoModal(parentWindow);
        if (result <= 0)
            return 0;

        return m_eventId;
    }

    long& GetEventId() noexcept
    {
        return m_eventId;
    }

private:
    PropSheetHeader             m_propertySheetHeader;
    std::vector<HPROPSHEETPAGE> m_pages;
    long                        m_eventId{};
};

} // end msf namespace.
