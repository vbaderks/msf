//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include <msf.h>


class __declspec(novtable) ShellFolderViewCB :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public MSF::ShellFolderViewCBImpl<ShellFolderViewCB>
{
public:
    static ATL::CComPtr<IShellFolderViewCB> CreateInstance(PCUIDLIST_RELATIVE folder)
    {
        ATL::CComObject<ShellFolderViewCB>* instance;
        HRESULT hr = ATL::CComObject<ShellFolderViewCB>::CreateInstance(&instance);
        if (FAILED(hr))
            MSF::RaiseException();

        ATL::CComPtr<IShellFolderViewCB> shellfolderviewcb(instance);
        instance->SetFolder(folder);
        return shellfolderviewcb;
    }

    DECLARE_NOT_AGGREGATABLE(ShellFolderViewCB)

    BEGIN_COM_MAP(ShellFolderViewCB)
        COM_INTERFACE_ENTRY(IShellFolderViewCB)
        COM_INTERFACE_ENTRY(IFolderViewSettings)
    END_COM_MAP()

    ShellFolderViewCB() :
        ShellFolderViewCBImpl<ShellFolderViewCB>(SHCNE_RENAMEITEM |
                                                  SHCNE_RENAMEFOLDER |
                                                  SHCNE_DELETE |
                                                  SHCNE_ATTRIBUTES |
                                                  SHCNE_UPDATEDIR |
                                                  SHCNE_CREATE |
                                                  SHCNE_UPDATEITEM)
    {
    }

    HRESULT OnGetButtonInfo(TBINFO* /*ptbinfo*/)
    {
        /*
        ptbinfo->cbuttons = 2;
        ptbinfo->uFlags   = TBIF_DEFAULT | TBIF_APPEND;
        return S_OK;
        */
        return E_FAIL;
    }

    HRESULT OnGetButtons(unsigned short /*idCmdFirst*/, unsigned short /*cbtnMax*/, TBBUTTON* /*ptbbutton*/)
    {
        /*
        ATLASSERT(cbtnMax == 2);

        ptbbutton[0].idCommand = idCmdFirst;
        ptbbutton[0].iBitmap   = I_IMAGENONE;
        ptbbutton[0].fsState   = TBSTATE_ENABLED;
        ptbbutton[0].fsStyle   = BTNS_AUTOSIZE | BTNS_SHOWTEXT;
        ptbbutton[0].iString   = IDS_CONTEXTMENU_CAPTION;

        ptbbutton[1].idCommand = idCmdFirst + 1;
        ptbbutton[1].iBitmap   = I_IMAGENONE;
        ptbbutton[1].fsState   = TBSTATE_ENABLED;
        ptbbutton[1].fsStyle   = BTNS_AUTOSIZE | BTNS_SHOWTEXT;
        ptbbutton[1].iString   = IDS_CONTEXTMENU_CAPTION;

        return S_OK;
        */
        return E_FAIL;
    }

    HRESULT OnInvokeCommand(unsigned int idCmd)
    {
        switch (idCmd)
        {
        case 0:
            break;

        case 1:
            break;

        default:
            break;
        }

        return S_OK;
    }
};
