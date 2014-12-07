//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

// Enabled improved type checking of the Win32 SDK header files.
#define STRICT

// Allow use of features specific to Windows 7 or later.
#define _WIN32_WINNT 0x0601

// Allow use of features specific to Windows 98 or later.
// Change this to the appropriate value to target Windows Me or later.
#define _WIN32_WINDOWS 0x0410

// Allow use of features specific to IE.
#define _WIN32_IE 0x0501  // Target IE 5.01 (required for definition of shell interfaces)

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
