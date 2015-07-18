// Enabled improved type checking of the Win32 SDK header files.
#define STRICT

// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501

// Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501

// Allow use of features specific to Windows 98 or later.
// Change this to the appropriate value to target Windows Me or later.
#define _WIN32_WINDOWS 0x0410

// Allow use of features specific to IE.
#define _WIN32_IE 0x0501  // Target IE 5.01 (required for definition of shell interfaces)

// Enable Windows XP Side by Side functionality to use the new visual styles.
// Note: MessageBox is not redefined by winuser.inl (by design): need to call explicit IsolationAwareMessageBox
#define ISOLATION_AWARE_ENABLED 1

#define _ATL_APARTMENT_THREADED

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS // some CString constructors will be explicit

#ifdef _DEBUG
#define _ATL_DEBUG_QI // Trace which interfaces are requested.
#endif

// Disable not usefull warnings.
#pragma warning(disable: 4505) // unreferenced local function has been removed

#include "../include/msfbase.h"

#define _ATL_EX_CONVERSION_MACROS_ONLY // Only use the new Unicode <-> Ansi conversion macro's.

using namespace std;
using namespace MSF;
