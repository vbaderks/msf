//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../include/contextcommand.h"
#include "../include/version.h"
#include "resource.h"


class CAboutMSFCommand : public CContextCommand
{
public:
	virtual void operator()(const CMINVOKECOMMANDINFO* pici,
	                        const std::vector<CString>& /* filenames */)
	{
		CString strText;
		strText.Format(IDS_CONTEXTMENU_ABOUT_MASK, HIWORD(MSF_VER), LOWORD(MSF_VER));

		IsolationAwareMessageBox(pici->hwnd, strText, LoadString(IDS_CONTEXTMENU_CAPTION), MB_OK);
	}
};
