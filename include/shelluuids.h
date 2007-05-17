//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


// The Win32 SDK header files don't attach GUIDs to some of the shell 
// interfaces or sometimes the visual C++ comdef.h file hides the SDK comdef.h file

struct __declspec(uuid("000214E4-0000-0000-C000-000000000046")) IContextMenu;
struct __declspec(uuid("000214F4-0000-0000-C000-000000000046")) IContextMenu2;
struct __declspec(uuid("BCFCE0A0-EC17-11D0-8D10-00A0C90F2719")) IContextMenu3;
struct __declspec(uuid("000214E5-0000-0000-C000-000000000046")) IShellIcon;
struct __declspec(uuid("7D688A70-C613-11D0-999B-00C04FD655E1")) IShellIconOverlay;
struct __declspec(uuid("2047E320-F2A9-11CE-AE65-08002B2E1262")) IShellFolderViewCB;
struct __declspec(uuid("000214EC-0000-0000-C000-000000000046")) IShellDetails;
struct __declspec(uuid("CEF04FDF-FE72-11D2-87A5-00C04F6837CF")) IPersistFolder3;
struct __declspec(uuid("E8025004-1C42-11D2-BE2C-00A0C9A83DA1")) IColumnProvider;
struct __declspec(uuid("00021500-0000-0000-C000-000000000046")) IQueryInfo;
struct __declspec(uuid("000214EB-0000-0000-C000-000000000046")) IExtractIconA;
struct __declspec(uuid("000214FA-0000-0000-C000-000000000046")) IExtractIconW;
struct __declspec(uuid("000214EF-0000-0000-C000-000000000046")) ICopyHookA;
struct __declspec(uuid("000214FC-0000-0000-C000-000000000046")) ICopyHookW;
