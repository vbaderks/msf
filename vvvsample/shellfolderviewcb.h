//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../include/shellfolderviewcbimpl.h"
#include "../include/taskband.h"
#include "resource.h"


class ATL_NO_VTABLE CShellFolderViewCB :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IShellFolderViewCBImpl<CShellFolderViewCB>
{
public:
    static CComPtr<IShellFolderViewCB> CreateInstance(LPCITEMIDLIST pidlFolder)
    {
        CComObject<CShellFolderViewCB>* pinstance;
        HRESULT hr = CComObject<CShellFolderViewCB>::CreateInstance(&pinstance);
        if (FAILED(hr))
            RaiseException();

        CComPtr<IShellFolderViewCB> shellfolderviewcb(pinstance);
        pinstance->SetFolder(pidlFolder);
        return shellfolderviewcb;
    }


    DECLARE_NOT_AGGREGATABLE(CShellFolderViewCB)


    BEGIN_COM_MAP(CShellFolderViewCB)
        COM_INTERFACE_ENTRY(IShellFolderViewCB)
    END_COM_MAP()


    CShellFolderViewCB() :
        IShellFolderViewCBImpl<CShellFolderViewCB>(SHCNE_RENAMEITEM |
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
