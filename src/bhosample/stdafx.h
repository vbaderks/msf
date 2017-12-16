// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#define _ATL_ALL_WARNINGS // turns off ATL's hiding of some common and often safely ignored warning messages
#ifdef _DEBUG
#define _ATL_DEBUG_QI // Trace which interfaces are requested.
#endif

// Disable not usefull Wall warnings
#pragma warning(disable: 4505) // unreferenced local function has been removed.

#include "../include/msfbase.h"

#pragma warning(push)
#pragma warning(disable: 4127) // warning C4127: conditional expression is constant
#include <atlutil.h>
#pragma warning(pop)

// Win32 include files.
#include <mshtml.h>
#include <exdispid.h>
#include <wincred.h>
#pragma comment(lib, "credui.lib")


using namespace msf;
using namespace std;
