//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


// Purpose: emulated version of the system shell folder view
//          As this version has full source it makes things easier to understand.

// TODO: make this CDefView at least do the 'view' part.

#include "msfbase.h"
#include "shellviewimpl.h"
#include "sfvmdefines.h"


namespace MSF
{

class CCallback
{
public:
    explicit CCallback(IShellFolderViewCB* pshellfolderviewcb) :
        _rshellfolderviewcb(pshellfolderviewcb)
    {
    }


    HRESULT CallCB(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (!_rshellfolderviewcb)
            return E_FAIL;

        return _rshellfolderviewcb->MessageSFVCB(uMsg, wParam, lParam);
    }


    // Purpose: function is used when shellfolder view is created to 
    //          obtain something.. [UNDOCUMENTED]
    HRESULT GetUndocumentedSetting()
    {
        return CallCB(SFVM_UNDOCUMENTED78, 0, 0);
    }


    // Purpose: used during startup to retrieve sorting defaults.
    HRESULT GetSortDefaults(int* piDirection, int* piColumn)
    {
        return CallCB(SFVM_GETSORTDEFAULTS,
            reinterpret_cast<WPARAM>(piDirection), reinterpret_cast<LPARAM>(piColumn));
    }


    HRESULT GetViewData()
    {
        return CallCB(SFVM_GET_VIEW_DATA, 0, 0);
    }


    HRESULT GetShowWebView()
    {
        return CallCB(SFVM_SHOWWEBVIEW, 0, 0);
    }


    HRESULT GetBackGroundEnum()
    {
        return CallCB(SFVM_BACKGROUNDENUM, 0, 0);
    }


    HRESULT GetColumnStream()
    {
        return CallCB(SFVM_GETCOLUMNSTREAM, 0, 0);
    }


    HRESULT GetWebViewLayout()
    {
        return CallCB(SFVM_GET_WEBVIEW_LAYOUT, 0, 0);
    }


    HRESULT GetCustomViewInfo()
    {
        return CallCB(SFVM_GET_CUSTOMVIEWINFO, 0, 0);
    }


    HRESULT FindItem()
    {
        return CallCB(SFVM_FINDITEM, 0, 0);
    }


    void NotifyWndMain(HWND hwnd)
    {
        CallCB(SFVM_WNDMAIN, hwnd, 0);
    }


    void NotifyWndSize()
    {
        CallCB(SFVM_SIZE, 0, 0);
    }


    void NotifyEnumeratedItems()
    {
        CallCB(SFVM_ENUMERATEDITEMS, 0, 0);
    }


    void NotifyWindowCreated(HWND hwndView)
    {
        CallCB(SFVM_WINDOWCREATED, hwndView, 0);
    }


    void NotifyWindowClosing()
    {
        CallCB(SFVM_WINDOWCLOSING, 0, 0);
    }


    void NotifyRemovingObject()
    {
        CallCB(SFVM_REMOVINGOBJECT, 0, 0);
    }


    void NotifyViewDestruct()
    {
        CallCB(SFVM_VIEWRELEASE, 0, 0);
    }

private:
    CComPtr<IShellFolderViewCB> _rshellfolderviewcb;
};


class CDefviewEnumTask()
{
public:
    FillObjectsToDPA()
    {
        _callback.GetBackGroundEnum();

        //_callback. // enum done?
    }
}


class CViewState
{
public:
    void InitWithDefaults(const CCallback& callback)
    {
        GetDefaults(callback);
    }


    void GetDefaults(const CCallback& callback)
    {
        if (SUCCEEDED(callback.GetSortDefaults(&_iDirection, &_iColumn)))
            return;
    
        _iDirection = 0;// todo
        _iColumn = 0;
    }

    int _iDirection;
    int _iColumn;
};


class CContextMenuCB
{
    void CallBack()
    {
    }


    NotifyDfmCommand()
    {
        CallBack();
    }
}


class __declspec(novtable) CDefView : public IShellViewImpl<CDefView>
{
public:
    DECLARE_NOT_AGGREGATABLE(CDefView)

    BEGIN_COM_MAP(CDefView)
        COM_INTERFACE_ENTRY(IShellView)
    END_COM_MAP()

    // COM_INTERFACE_ENTRY(IOleWindow) ?


    static CComPtr<IShellView> CreateInstance(const SFV_CREATE* pcsfv)
    {
        CComObject<CDefView>* pinstance;
        RaiseExceptionIfFailed(
            CComObject<CDefView>::CreateInstance(&pinstance));

        CComPtr<IShellView> rshellview(pinstance);

        pinstance->Init(pcsfv);

        return rshellview;
    }


    CDefView()
    {
    }


    void Init(const SFV_CREATE* pcsfv)
    {
        _rshellfolder       = pcsfv->pshf;
        _rshellviewOuter    = pcsfv->psvOuter;
        _rshellfolderviewcb = pcsfv->psfvcb;
    }


    HRESULT CreateViewWindow(IShellView* psvPrevious,
                             const FOLDERSETTINGS* pfs,
                             IShellBrowser* psb,
                             RECT* prcView,
                             HWND* phWnd)
    {
        //ATLTRACENOTIMPL(L"IShellViewImpl::CreateViewWindow");
        SV2CVW2_PARAMS params;

        params.cbSize   = sizeof(params);
        params.psvPrev  = psvPrevious;
        params.pfs      = pfs;
        params.psbOwner = psb;
        params.prcView  = prcView;
        params.pvid     = nullptr; // todo

        HRESULT hr = CreateViewWindow2(params);
        *phWnd = params.hwndView;
        return hr;
    }


    HRESULT CreateViewWindow2(SV2CVW2_PARAMS* pParams)
    {
        _callback.NotifyWndMain(0);

        FillObjectsToDPA();

        SwitchToViewFVM();

        _callback.NotifyWindowCreated();

        UpdateListviewColors();

        _rshellbrowser = pParams.psbOwner;
    }


    void GetView()
    {
        ViewSupported();
    }


    void AddColumns()
    {
        //_callback.//streams
    }


    void DestroyViewWindow()
    {
        _callback.NotifyWindowClosing();
    }

private:

    void ViewSupported()
    {
        GetSFVMViewState();
    }


    void GetSFVMViewState()
    {
        _callback.GetViewData();
    }


    void SwitchToViewFVM()
    {
        AddColumns();

        TryShowWebView();
    }


    void TryShowWebView()
    {
        _callback.GetWebViewLayout();
    }


    static void WndProc()
    {
        // pthis->WndProc();
    }


    void WndProc()
    {
        switch()
        {
            case 
                OnCreate();

            case 
                WndSize();

            case 
                OnSelectionChanged();
        }
    }


    void OnCreate()
    {
        ShouldShowWebView();
    }


    void WndSize()
    {
        _callback.NotifyWndSize()
    }


    void OnSelectionChanged()
    {
    }


    bool ShouldShowWebView()
    {
        _callback.GetShowWebView();
    }


    void FindItem()
    {
        _callback.FindItem();
    }


    void ExplorerCommand()
    {
        InvokeContextMenu();
    }


    void InvokeContextMenu()
    {
        _deffoldermenu.InvokeCommand(_callback);
    }

    //WndProc()
    //OnFSNotify()
    //AddOrUpdateItem()
    //UpdateStatusBar()
    //OnLVNotify()
    //Attributes()
    //IsImageMode()
    //SetSortFeedback()
    //SetDefaultViewSettings
    //ForwardMenuMessages
    //SetFolderColors
    //OnInitMenuPopup
    //OnClipboardChange

    // Member variables
    CComPtr<IShellFolder>       _rshellfolder;
    CComPtr<IShellView>         _rshellviewOuter;
    CComPtr<IShellFolderViewCB> _rshellfolderviewcb;
    CComPtr<IShellBrowser>      _rshellbrowser;
    ATL::CWindow                _window;
};

} // namespace MSF
