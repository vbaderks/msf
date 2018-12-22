//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include <shlobj.h>

// Undocumented IShellFolderViewCB callback notifications IDs.
// Most info was collected from public domain info on the Internet.
constexpr uint32_t SFVM_SELECTIONCHANGED            =   8;
constexpr uint32_t SFVM_DRAWMENUITEM                =   9;
constexpr uint32_t SFVM_MEASUREMENUITEM             =  10;
constexpr uint32_t SFVM_EXITMENULOOP                =  11; // generated on win9x
constexpr uint32_t SFVM_VIEWRELEASE                 =  12;
constexpr uint32_t SFVM_GETNAMELENGTH               =  13;
constexpr uint32_t SFVM_WINDOWCLOSING               =  16;
constexpr uint32_t SFVM_LISTREFRESHED               =  17;
constexpr uint32_t SFVM_WINDOWFOCUSED               =  18;
constexpr uint32_t SFVM_CREATEVIEWWND2              =  20; // or SFVM_REGISTERCOPYHOOK ?
constexpr uint32_t SFVM_COPYHOOKCALLBACK            =  21;
constexpr uint32_t SFVM_UNMERGEFROMMENU             =  28;
constexpr uint32_t SFVM_ADDINGOBJECT                =  29;
constexpr uint32_t SFVM_REMOVINGOBJECT              =  30;
constexpr uint32_t SFVM_GETCOMMANDDIR               =  33; // or SFVM_PREINVOKECOMMAND ?
constexpr uint32_t SFVM_GETCOLUMNSTREAM             =  34;
constexpr uint32_t SFVM_CANSELECTALL                =  35;
constexpr uint32_t SFVM_ISSTRICTREFRESH             =  37;
constexpr uint32_t SFVM_ISCHILDOBJECT               =  38;
constexpr uint32_t SFVM_GETEXTVIEWS                 =  40;
constexpr uint32_t SFVM_FINDITEM                    =  44;
constexpr uint32_t SFVM_WNDMAIN                     =  46;
constexpr uint32_t SFVM_SORT                        =  50;
constexpr uint32_t SFVM_UNDOCUMENTED51              =  51; // SFVM_STANDARDVIEWS?
constexpr uint32_t SFVM_UNDOCUMENTED56              =  56; // seen on win9x
constexpr uint32_t SFVM_UNDOCUMENTED60              =  60; // seen on win9x
constexpr uint32_t SFVM_GETPERSISTHISTORY           =  67;
constexpr uint32_t SFVM_WNDPROC                     =  74;
constexpr uint32_t SFVM_SHOWWEBVIEW                 =  75;
constexpr uint32_t SFVM_GET_WEBVIEW_TEMPLATE        =  76; // Called by shell32::OnGetWebViewTemplate
constexpr uint32_t SFVM_GET_CUSTOMVIEWINFO          =  77;
constexpr uint32_t SFVM_UNDOCUMENTED78              =  78; // OnGetInitDefaults (called always during init, just before getsortdefaults)
constexpr uint32_t SFVM_ENUMERATEDITEMS             =  79;
constexpr uint32_t SFVM_GET_VIEW_DATA               =  80;
constexpr uint32_t SFVM_GET_VIEWINFO_TEMPLATE       =  81; // Called by shell32::GetSFVMViewInfoTemplate
constexpr uint32_t SFVM_GET_WEBVIEW_LAYOUT          =  82;
constexpr uint32_t SFVM_GET_WEBVIEW_CONTENT         =  83;
constexpr uint32_t SFVM_GET_WEBVIEW_TASKS           =  84;
constexpr uint32_t SFVM_GET_WEBVIEW_THEME           =  86;
constexpr uint32_t SFVM_GETDEFERREDVIEWSETTINGS     =  92;
constexpr uint32_t SFVM_GET_FOLDER_TYPE_DESCRIPTION =  94; // seen on Windows 8, name based on call stack analysis
constexpr uint32_t SFVM_POST_CREATE_VIEW_WINDOW3    =  99; // seen on Windows 8, name based on call stack analysis
constexpr uint32_t SFVM_PRE_CREATE_VIEW_WINDOW3     = 100; // seen on Windows 8, name based on call stack analysis
constexpr uint32_t SFVM_GET_EMPTY_TEXT              = 101; // seen on Windows 8, name based on call stack analysis
constexpr uint32_t SFVM_GET_MAX_ITEM_COUNT          = 102; // seen on Windows 8, name based on call stack analysis
constexpr uint32_t SFVM_CANCEL_PENDING_NAV          = 103; // seen on Windows 8, name based on call stack analysis
constexpr uint32_t SFVM_REGISTER_FOR_CHANGES        = 106; // seen on Windows 8, name based on call stack analysis
