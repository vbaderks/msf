//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#pragma once

#include "vvvitem.h"
#include "resource.h"
#include <msf.h>
#include <atlsnap.h>


class PropertyPageItem : public ATL::CSnapInPropertyPageImpl<PropertyPageItem>
{
public:
    static HPROPSHEETPAGE CreateInstance(VVVItem& item, long& wEventId, IShellFolder* shellFolder)
    {
        auto page = std::make_unique<PropertyPageItem>(item, wEventId, shellFolder);
        const auto result = page->Create();
        if (result)
        {
            // Page will be deleted by ATL base class when the property sheet is done with it.
            page.release(); // -V530
        }
        return result;
    }

    enum { IDD = IDD_PROPERTY_PAGE_ITEM };


#pragma warning(push)
#pragma warning(disable: 26433) // use override
    BEGIN_MSG_MAP(PropertyPageItem)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        CHAIN_MSG_MAP(CSnapInPropertyPageImpl<PropertyPageItem>)
    END_MSG_MAP()
#pragma warning(pop)

    PropertyPageItem(VVVItem& item, long& wEventId, IShellFolder* shellFolder) :
        m_item(item),
        m_eventId(wEventId),
        m_shellFolder(shellFolder)
    {
    }

    ~PropertyPageItem() = default;
    PropertyPageItem(const PropertyPageItem&) = delete;
    PropertyPageItem(PropertyPageItem&&) = delete;
    PropertyPageItem& operator=(const PropertyPageItem&) = delete;
    PropertyPageItem& operator=(PropertyPageItem&&) = delete;

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        InitializeStaticString();
        InitializeControls();
        return 1;
    }

    BOOL OnApply()
    {
        m_eventId = 0;
        const std::wstring name{GetEditItemName()};

        if (m_item.GetDisplayName() != name)
        {
            m_eventId |= SHCNE_RENAMEITEM;
        }

        const unsigned int nSize = GetDlgItemInt(IDC_EDIT_ITEM_SIZE, nullptr, false);
        if (m_item.GetSize() != nSize)
        {
            m_eventId |= SHCNE_ATTRIBUTES;
        }

        if (m_eventId != 0)
        {
            m_pidlNew.Attach(VVVItem::CreateItemIdList(m_item.GetID(),
                                                       nSize, m_item.IsFolder(), name));
        }

        return true;
    }

private:
    // It is easier to patch the dialog during runtime with the locale
    // strings then to maintain copies of the dialog resource.
    void InitializeStaticString()
    {
        ATLVERIFY(SetDlgItemText(IDC_STATIC_ITEM_NAME, (msf::LoadResourceString(IDS_SHELLEXT_NAME) + L":").c_str()));
        ATLVERIFY(SetDlgItemText(IDC_STATIC_ITEM_SIZE, (msf::LoadResourceString(IDS_SHELLEXT_SIZE) + L":").c_str()));
    }

    void InitializeControls()
    {
        ATLVERIFY(SetDlgItemText(IDC_EDIT_ITEM_NAME, m_item.GetName().c_str()));
        ATLVERIFY(SetDlgItemInt(IDC_EDIT_ITEM_SIZE, m_item.GetSize()));

        ATLVERIFY(SUCCEEDED(SHLimitInputEdit(GetDlgItem(IDC_EDIT_ITEM_NAME), m_shellFolder)));
    }

    std::wstring GetEditItemName() const
    {
        ATL::CString strName;
        static_cast<void>(GetDlgItemText(IDC_EDIT_ITEM_NAME, strName));
        strName.Trim();
        return strName.GetString();
    }

    // Member variables
    const VVVItem&        m_item;
    msf::ItemIDList       m_pidlNew;
    long&                 m_eventId;
    ATL::CComPtr<IShellFolder> m_shellFolder;
};
