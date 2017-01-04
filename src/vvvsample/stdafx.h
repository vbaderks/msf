//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

#pragma once

// To support testing with the /Wall switch certain warning must be disabled when including
// Win32, C\C++ runtime or ATL header files.

#ifdef WALL_TESTING
#pragma warning(disable: 4226) // '': assignment operator was implicitly defined as deleted (VS2015)
#pragma warning(disable: 4263) // 'function' : member function does not override any base class virtual member function (VS2015)
#pragma warning(disable: 4264) // auto generated after C4263 (VS2015)
#pragma warning(disable: 4365) // 'argument' conversion from 'unsigned int' to 'int' (VS2015)
#pragma warning(disable: 4464) // relative include path contains '..' (VS2015)
#pragma warning(disable: 4514) // '...': unreferenced inline function has been removed (VS2015)
#pragma warning(disable: 4710) // '...': function not inlined (VS2015)
#pragma warning(disable: 4820) // 'x' bytes padding added after data member. (VS2015)
#endif

// Enabled improved type checking of the Win32 SDK header files.
#define STRICT

// Allow use of features specific to Windows 7 or later.
#define _WIN32_WINNT 0x0601

// Enable Windows XP Side by Side functionality to use the new visual styles.
// Note: MessageBox is not redefined by winuser.inl (by design): need to call explicit IsolationAwareMessageBox
#define ISOLATION_AWARE_ENABLED 1

// Shell Extensions are COM apartment threaded.
#define _ATL_APARTMENT_THREADED

// VVV sample is ANSI C++ complaint. Enabled ATL PTM (pointer to a class member function ) warnings.
#define _ATL_ENABLE_PTM_WARNING

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS // some CString constructors will be explicit

#ifdef _DEBUG
#define _ATL_DEBUG_QI // Trace which interfaces are requested.
#endif

#define _ATL_EX_CONVERSION_MACROS_ONLY // Only use the new Unicode <-> Ansi conversion macro's.

#define _ATL_NO_HOSTING // will prevent including mshtml.h, which is not /Zc:strictStrings compatible.

#include "..\include\msfbase.h"

// Disable not usefull warnings.
#pragma warning(disable: 4505) // unreferenced local function has been removed


using namespace std;
using namespace MSF;


PCWSTR const wszVVVFileRootExt     = L"VVVFile";
PCWSTR const wszVVVExtension       = L".vvv";
const wchar_t* const tszVVVExtension =L".vvv";

// Artificial limit to prevent allocating of to much memory. (remove if required)
const unsigned int MAX_VVV_ITEM_SIZE = 3000;
