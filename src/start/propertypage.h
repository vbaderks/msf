#pragma once
#include "../include/propertypageimpl.h"
#include "../include/strutil.h"
#include "../include/util.h"
#include "resource.h"


class CPropertyPage : public CShellExtPropertyPageImpl<CPropertyPage>
{
public:
	static HPROPSHEETPAGE CreateInstance(/* optional arguments */)
	{
		CPropertyPage* ppage = new CPropertyPage(/* optional arguments */);
		return ppage->Create();
	}

	enum { IDD = IDD_PROPERTY_PAGE };

	BEGIN_MSG_MAP(CPropertyPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CShellExtPropertyPageImpl<CPropertyPage>)
	END_MSG_MAP()

	CPropertyPage(/* optional arguments */)
	{
	}


	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// TODO: add code to initialise the dialogbox controls.
		return 1;
	}


	BOOL OnApply()
	{
		// TODO: implement.
		return true;
	}
};
