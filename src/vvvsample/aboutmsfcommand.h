//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "resource.h"
#include <msf.h>


class AboutMSFCommand : public MSF::ContextMenuCommand
{
public:
    void operator()(const CMINVOKECOMMANDINFO* pici, const std::vector<std::wstring>& /* filenames */) override
    {
        IsolationAwareMessageBox(pici->hwnd,
                                 MSF::FormatResourceMessage(IDS_CONTEXTMENU_ABOUT_MASK, HIWORD(MSF_VER), LOWORD(MSF_VER)).c_str(),
                                 MSF::LoadResourceString(IDS_CONTEXTMENU_CAPTION).c_str(), MB_OK);
    }
};
