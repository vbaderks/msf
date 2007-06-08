//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once
#include <atlsnap.h>
#include "../include/strutil.h"
#include "../include/util.h"
#include "../include/pidl.h"
#include "vvvitem.h"
#include "resource.h"


class CPropertyPageItem : public CSnapInPropertyPageImpl<CPropertyPageItem>
{
public:
	static HPROPSHEETPAGE CreateInstance(CVVVItem& item, long& wEventId, IShellFolder* pshellfolder)
	{
		CPropertyPageItem* ppage = new CPropertyPageItem(item, wEventId, pshellfolder);
		return ppage->Create();
	}

	enum { IDD = IDD_PROPERTY_PAGE_ITEM };

	BEGIN_MSG_MAP(CPropertyPageItem)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CSnapInPropertyPageImpl<CPropertyPageItem>)
	END_MSG_MAP()


	CPropertyPageItem(CVVVItem& item, long& wEventId, IShellFolder* pshellfolder) :
		m_item(item),
		m_wEventId(wEventId),
		m_rshellfolder(pshellfolder)
	{
	}


	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		InitializeStaticString();
		InitializeControls();
		return 1;
	}


	BOOL OnApply()
	{
		m_wEventId = 0;

		CString strName;
		GetDlgItemText(IDC_EDIT_ITEM_NAME, strName);
		strName.Trim();
		if (m_item.GetDisplayName() != strName)
		{
			m_wEventId |= SHCNE_RENAMEITEM;
		}

		unsigned int nSize = GetDlgItemInt(IDC_EDIT_ITEM_SIZE, NULL, false);
		if (m_item.GetSize() != nSize)
		{
			m_wEventId |= SHCNE_ATTRIBUTES;
		}

		if (m_wEventId != 0)
		{
			m_pidlNew.Attach(CVVVItem::CreateItemIdList(m_item.GetID(),  
				nSize, m_item.IsFolder(), strName));
		}	

		return true;
	}

private:
	// It is easier to patch the dialog during runtime with the locale
	// strings then to maintain copies of the dialog resource.
	void InitializeStaticString()
	{
		ATLVERIFY(SetDlgItemText(IDC_STATIC_ITEM_NAME, LoadString(IDS_SHELLEXT_NAME) + _T(":")));
		ATLVERIFY(SetDlgItemText(IDC_STATIC_ITEM_SIZE, LoadString(IDS_SHELLEXT_SIZE) + _T(":")));
	}


	void InitializeControls()
	{
		ATLVERIFY(SetDlgItemText(IDC_EDIT_ITEM_NAME, m_item.GetName()));
		ATLVERIFY(SetDlgItemInt(IDC_EDIT_ITEM_SIZE, m_item.GetSize()));

		// Note: SHLimitInputEdit is only supported on Windows XP.
		//ATLVERIFY(SUCCEEDED(SHLimitInputEdit(GetDlgItem(IDC_EDITm_item_NAME), m_rshellfolder)));
	}

	CPropertyPageItem& operator=(const CPropertyPageItem&) throw();

	// Member variables
	const CVVVItem&       m_item;
	CPidl                 m_pidlNew;
	long&                 m_wEventId;
	CComPtr<IShellFolder> m_rshellfolder;
};
