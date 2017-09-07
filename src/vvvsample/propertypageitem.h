//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "../include/strutil.h"
#include "../include/pidl.h"
#include "vvvitem.h"
#include "resource.h"
#include <atlsnap.h>


class CPropertyPageItem : public ATL::CSnapInPropertyPageImpl<CPropertyPageItem>
{
public:
    static HPROPSHEETPAGE CreateInstance(VVVItem& item, long& wEventId, IShellFolder* pshellfolder)
    {
        auto ppage = new CPropertyPageItem(item, wEventId, pshellfolder);
        return ppage->Create();
    }

    enum { IDD = IDD_PROPERTY_PAGE_ITEM };

    BEGIN_MSG_MAP(CPropertyPageItem)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        CHAIN_MSG_MAP(CSnapInPropertyPageImpl<CPropertyPageItem>)
    END_MSG_MAP()

    CPropertyPageItem(VVVItem& item, long& wEventId, IShellFolder* pshellfolder) :
        m_item(item),
        m_wEventId(wEventId),
        m_rshellfolder(pshellfolder)
    {
    }

    CPropertyPageItem& operator=(const CPropertyPageItem&) = delete;

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        InitializeStaticString();
        InitializeControls();
        return 1;
    }

    BOOL OnApply()
    {
        m_wEventId = 0;

        ATL::CString strName;
        GetDlgItemText(IDC_EDIT_ITEM_NAME, strName);
        strName.Trim();
        std::wstring name(strName); // TODO

        if (m_item.GetDisplayName() != name)
        {
            m_wEventId |= SHCNE_RENAMEITEM;
        }

        unsigned int nSize = GetDlgItemInt(IDC_EDIT_ITEM_SIZE, nullptr, false);
        if (m_item.GetSize() != nSize)
        {
            m_wEventId |= SHCNE_ATTRIBUTES;
        }

        if (m_wEventId != 0)
        {
            m_pidlNew.Attach(VVVItem::CreateItemIdList(m_item.GetID(),
                nSize, m_item.IsFolder(), strName));
        }

        return true;
    }

private:
    // It is easier to patch the dialog during runtime with the locale
    // strings then to maintain copies of the dialog resource.
    void InitializeStaticString()
    {
        ATLVERIFY(SetDlgItemText(IDC_STATIC_ITEM_NAME, (MSF::LoadResourceString(IDS_SHELLEXT_NAME) + L":").c_str()));
        ATLVERIFY(SetDlgItemText(IDC_STATIC_ITEM_SIZE, (MSF::LoadResourceString(IDS_SHELLEXT_SIZE) + L":").c_str()));
    }

    void InitializeControls()
    {
        ATLVERIFY(SetDlgItemText(IDC_EDIT_ITEM_NAME, m_item.GetName().c_str()));
        ATLVERIFY(SetDlgItemInt(IDC_EDIT_ITEM_SIZE, m_item.GetSize()));

        // Note: SHLimitInputEdit is only supported on Windows XP.
        //ATLVERIFY(SUCCEEDED(SHLimitInputEdit(GetDlgItem(IDC_EDITm_item_NAME), m_rshellfolder)));
    }

    // Member variables
    const VVVItem &       m_item;
    MSF::CPidl            m_pidlNew;
    long &                m_wEventId;
    ATL::CComPtr<IShellFolder> m_rshellfolder;
};
