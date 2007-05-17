//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


// Undocumented DefFolderMenu callback notifications
// Discovered by Maksym Schipka, Axel Sommerfeldt, Henk Devos and myself.

// Most defines are defined in the latest shlobj.h of the Windows SDK 6.0 (Vista RC1 release)
// There are here duplicated to use the compile with VC++ 7.1 and VC++ 8.0

#define DFM_MERGECONTEXTMENU         1      // uFlags       LPQCMINFO
#define DFM_INVOKECOMMAND            2      // idCmd        pszArgs
#define DFM_CREATE                   3
#define DFM_DESTROY                  4      // Fired during the destruction of the menu (seen on XP).
#define DFM_GETHELPTEXT              5      // idCmd,cchMax pszText -Ansi
#define DFM_WM_MEASUREITEM           6      // ---from the message---
#define DFM_WM_DRAWITEM              7      // ---from the message---
#define DFM_WM_INITMENUPOPUP         8      // ---from the message---
#define DFM_VALIDATECMD              9      // idCmd        0
#define DFM_MERGECONTEXTMENU_TOP     10     // uFlags       LPQCMINFO
#define DFM_GETHELPTEXTW             11     // idCmd,cchMax pszText -Unicode
#define DFM_INVOKECOMMANDEX          12     // idCmd        PDFMICS
#define DFM_MAPCOMMANDNAME           13     // idCmd *      pszCommandName
#define DFM_GETDEFSTATICID           14     // idCmd *      0
#define DFM_GETVERBW                 15     // idCmd,cchMax pszText -Unicode
#define DFM_GETVERBA                 16     // idCmd,cchMax pszText -Ansi
#define DFM_MERGECONTEXTMENU_BOTTOM  17     // uFlags       LPQCMINFO

// Commands from DFM_INVOKECOMMAND when strings are passed in
#define DFM_CMD_DELETE          ((UINT)-1)
#define DFM_CMD_MOVE            ((UINT)-2)
#define DFM_CMD_COPY            ((UINT)-3)
#define DFM_CMD_LINK            ((UINT)-4)
#define DFM_CMD_PROPERTIES      ((UINT)-5)
#define DFM_CMD_NEWFOLDER       ((UINT)-6)
#define DFM_CMD_PASTE           ((UINT)-7)
#define DFM_CMD_VIEWLIST        ((UINT)-8)
#define DFM_CMD_VIEWDETAILS     ((UINT)-9)
#define DFM_CMD_PASTELINK       ((UINT)-10)
#define DFM_CMD_PASTESPECIAL    ((UINT)-11)
#define DFM_CMD_MODALPROP       ((UINT)-12)
#define DFM_CMD_RENAME          ((UINT)-13)
