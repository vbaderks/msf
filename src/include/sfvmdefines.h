//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include <shlobj.h>

// Undocumented IShellFolderViewCB callback notifications IDs.
// Most info was collected from public domain info on the Internet.
constexpr UINT SFVM_SELECTIONCHANGED            =   8;
constexpr UINT SFVM_DRAWMENUITEM                =   9;
constexpr UINT SFVM_MEASUREMENUITEM             =  10;
constexpr UINT SFVM_EXITMENULOOP                =  11; // generated on win9x
constexpr UINT SFVM_VIEWRELEASE                 =  12;
constexpr UINT SFVM_GETNAMELENGTH               =  13;
constexpr UINT SFVM_WINDOWCLOSING               =  16;
constexpr UINT SFVM_LISTREFRESHED               =  17;
constexpr UINT SFVM_WINDOWFOCUSED               =  18;
constexpr UINT SFVM_CREATEVIEWWND2              =  20; // or SFVM_REGISTERCOPYHOOK ?
constexpr UINT SFVM_COPYHOOKCALLBACK            =  21;
constexpr UINT SFVM_UNMERGEFROMMENU             =  28;
constexpr UINT SFVM_ADDINGOBJECT                =  29;
constexpr UINT SFVM_REMOVINGOBJECT              =  30;
constexpr UINT SFVM_GETCOMMANDDIR               =  33; // or SFVM_PREINVOKECOMMAND ?
constexpr UINT SFVM_GETCOLUMNSTREAM             =  34;
constexpr UINT SFVM_CANSELECTALL                =  35;
constexpr UINT SFVM_ISSTRICTREFRESH             =  37;
constexpr UINT SFVM_ISCHILDOBJECT               =  38;
constexpr UINT SFVM_GETEXTVIEWS                 =  40;
constexpr UINT SFVM_FINDITEM                    =  44;
constexpr UINT SFVM_WNDMAIN                     =  46;
constexpr UINT SFVM_SORT                        =  50;
constexpr UINT SFVM_UNDOCUMENTED51              =  51; // SFVM_STANDARDVIEWS?
constexpr UINT SFVM_UNDOCUMENTED56              =  56; // seen on win9x
constexpr UINT SFVM_UNDOCUMENTED60              =  60; // seen on win9x
constexpr UINT SFVM_GETPERSISTHISTORY           =  67;
constexpr UINT SFVM_WNDPROC                     =  74;
constexpr UINT SFVM_SHOWWEBVIEW                 =  75;
constexpr UINT SFVM_GET_WEBVIEW_TEMPLATE        =  76; // Called by shell32::OnGetWebViewTemplate
constexpr UINT SFVM_GET_CUSTOMVIEWINFO          =  77;
constexpr UINT SFVM_UNDOCUMENTED78              =  78; // OnGetInitDefaults (called always during init, just before getsortdefaults)
constexpr UINT SFVM_ENUMERATEDITEMS             =  79;
constexpr UINT SFVM_GET_VIEW_DATA               =  80;
constexpr UINT SFVM_GET_VIEWINFO_TEMPLATE       =  81; // Called by shell32::GetSFVMViewInfoTemplate
constexpr UINT SFVM_GET_WEBVIEW_LAYOUT          =  82;
constexpr UINT SFVM_GET_WEBVIEW_CONTENT         =  83;
constexpr UINT SFVM_GET_WEBVIEW_TASKS           =  84;
constexpr UINT SFVM_GET_WEBVIEW_THEME           =  86;
constexpr UINT SFVM_GETDEFERREDVIEWSETTINGS     =  92;
constexpr UINT SFVM_GET_FOLDER_TYPE_DESCRIPTION =  94; // seen on Windows 8, name based on call stack analysis
constexpr UINT SFVM_POST_CREATE_VIEW_WINDOW3    =  99; // seen on Windows 8, name based on call stack analysis
constexpr UINT SFVM_PRE_CREATE_VIEW_WINDOW3     = 100; // seen on Windows 8, name based on call stack analysis
constexpr UINT SFVM_GET_EMPTY_TEXT              = 101; // seen on Windows 8, name based on call stack analysis
constexpr UINT SFVM_GET_MAX_ITEM_COUNT          = 102; // seen on Windows 8, name based on call stack analysis
constexpr UINT SFVM_CANCEL_PENDING_NAV          = 103; // seen on Windows 8, name based on call stack analysis
constexpr UINT SFVM_REGISTER_FOR_CHANGES        = 106; // seen on Windows 8, name based on call stack analysis
