//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "resource.h"

#include "../include/contextcommand.h"
#include "../include/strutil.h"
#include "../include/version.h"


class CAboutMSFCommand : public CContextCommand
{
public:
    void operator()(const CMINVOKECOMMANDINFO* pici, const std::vector<CString>& /* filenames */) override
    {
        CString strText;
        strText.Format(IDS_CONTEXTMENU_ABOUT_MASK, HIWORD(MSF_VER), LOWORD(MSF_VER));

        IsolationAwareMessageBox(pici->hwnd, strText, LoadString(IDS_CONTEXTMENU_CAPTION), MB_OK);
    }
};
