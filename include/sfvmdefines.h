//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include <shlobj.h>

// Undocumented IShellFolderViewCB callback notifications.
// Most info was collected from public domain info on the Internet.
const UINT SFVM_SELECTIONCHANGED  =  8;
const UINT SFVM_DRAWMENUITEM      =  9;
const UINT SFVM_MEASUREMENUITEM   = 10;
const UINT SFVM_EXITMENULOOP      = 11; // generated on win9x
const UINT SFVM_VIEWRELEASE       = 12;
const UINT SFVM_GETNAMELENGTH     = 13;
#ifndef SFVM_FSNOTIFY 
const UINT SFVM_FSNOTIFY          = 14; // defined in Feb 2003 SDK, but not in VC 2003 platform SDK.
#endif
const UINT SFVM_WINDOWCLOSING     = 16;
const UINT SFVM_LISTREFRESHED     = 17;
const UINT SFVM_WINDOWFOCUSED     = 18;
const UINT SFVM_CREATEVIEWWND2    = 20; // or SFVM_REGISTERCOPYHOOK ?
const UINT SFVM_COPYHOOKCALLBACK  = 21;
const UINT SFVM_UNMERGEFROMMENU   = 28;
const UINT SFVM_ADDINGOBJECT      = 29;
const UINT SFVM_REMOVINGOBJECT    = 30;
const UINT SFVM_GETCOMMANDDIR     = 33; // or SFVM_PREINVOKECOMMAND ?
const UINT SFVM_GETCOLUMNSTREAM   = 34;
const UINT SFVM_CANSELECTALL      = 35;
const UINT SFVM_ISSTRICTREFRESH   = 37;
const UINT SFVM_ISCHILDOBJECT     = 38;
const UINT SFVM_GETEXTVIEWS       = 40;
const UINT SFVM_FINDITEM          = 44;
const UINT SFVM_WNDMAIN           = 46;
const UINT SFVM_SORT              = 50;
const UINT SFVM_UNDOCUMENTED51    = 51; // SFVM_STANDARDVIEWS?
const UINT SFVM_UNDOCUMENTED56    = 56; // seen on win9x
const UINT SFVM_UNDOCUMENTED60    = 60; // seen on win9x
const UINT SFVM_GETPERSISTHISTORY = 67;

#ifndef SFVM_GETANIMATION
const UINT SFVM_GETANIMATION      = 68; // defined in Feb 2003 SDK, but not in VC 2003 platform SDK.
#endif

const UINT SFVM_WNDPROC                 = 74;
const UINT SFVM_SHOWWEBVIEW             = 75;
const UINT SFVM_GET_WEBVIEW_TEMPLATE    = 76; // Called by shell32::OnGetWebViewTemplate
const UINT SFVM_GET_CUSTOMVIEWINFO      = 77;
const UINT SFVM_UNDOCUMENTED78          = 78; // OnGetInitDefaults (called always during init, just before getsortdefaults)
const UINT SFVM_ENUMERATEDITEMS         = 79;
const UINT SFVM_GET_VIEW_DATA           = 80;
const UINT SFVM_GET_VIEWINFO_TEMPLATE   = 81; // Called by shell32::GetSFVMViewInfoTemplate
const UINT SFVM_GET_WEBVIEW_LAYOUT      = 82;
const UINT SFVM_GET_WEBVIEW_CONTENT     = 83;
const UINT SFVM_GET_WEBVIEW_TASKS       = 84;
const UINT SFVM_GET_WEBVIEW_THEME       = 86;
const UINT SFVM_GETDEFERREDVIEWSETTINGS = 92;
const UINT SFVM_GET_FOLDER_TYPE_DESCRIPTION = 94; // seen on Windows 8, name based on call stack analysis
const UINT SFVM_POST_CREATE_VIEW_WINDOW3= 99; // seen on Windows 8, name based on call stack analysis
const UINT SFVM_PRE_CREATE_VIEW_WINDOW3 = 100; // seen on Windows 8, name based on call stack analysis
const UINT SFVM_GET_EMPTY_TEXT          = 101; // seen on Windows 8, name based on call stack analysis
const UINT SFVM_GET_MAX_ITEM_COUNT      = 102; // seen on Windows 8, name based on call stack analysis
const UINT SFVM_CANCEL_PENDING_NAV      = 103; // seen on Windows 8, name based on call stack analysis
const UINT SFVM_REGISTER_FOR_CHANGES    = 106; // seen on Windows 8, name based on call stack analysis
