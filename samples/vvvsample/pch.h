//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//

#pragma once

// Allow use of features specific to Windows 7 or later.
#define _WIN32_WINNT 0x0601

// Enable Windows XP Side by Side functionality to use the new visual styles.
// Note: MessageBox is not redefined by winuser.inl (by design): need to call explicit IsolationAwareMessageBox
#define ISOLATION_AWARE_ENABLED 1

#ifdef _DEBUG
#define _ATL_DEBUG_QI // Trace which interfaces are requested.
#endif

#include <msf.h>

// Disable not useful warnings.
#pragma warning(disable: 4505) // unreferenced local function has been removed


PCWSTR const wszVVVFileRootExt     = L"VVVFile";
PCWSTR const wszVVVExtension       = L".vvv";
const wchar_t* const tszVVVExtension =L".vvv";

// Artificial limit to prevent allocating of to much memory. (remove if required)
constexpr unsigned int MAX_VVV_ITEM_SIZE = 3000;
