//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

// Enabled improved type checking of the Win32 SDK header files.
#define STRICT

// Allow use of features specific to Windows 95 and Windows NT 4 or later.
// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#define WINVER 0x0400

// Allow use of features specific to Windows NT 4 or later.
// Change this to the appropriate value to target Windows 2000 or later.
#define _WIN32_WINNT 0x0400

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

#define _ATL_ALL_WARNINGS // turns off ATL's hiding of some common and often safely ignored warning messages

#ifdef _DEBUG
#define _ATL_DEBUG_QI // Trace which interfaces are requested.
#endif


// Disable not usefull Wall warnings
#pragma warning(disable: 4061) // enumerate 'xxx' in switch of enum 'yyy' is not explicitly handled by a case label
#pragma warning(disable: 4820) // 'x' bytes padding added after member.
#pragma warning(disable: 4710) // function not inlined.
#pragma warning(disable: 4514) // unreferenced inline function has been removed.
#pragma warning(disable: 4711) // function xxx is selected for automatic inline expansion.
#pragma warning(disable: 4265) // class has virtual functions, but destructor is not virtual
#pragma warning(disable: 4505) // unreferenced local function has been removed.
#pragma warning(disable: 4928) // illegal copy-initialization; more than one user-defined conversion has been implicitly applied

#if _MSC_VER == 1300 // ATL 7.0 BEGIN_MSG_MAP generates: expression no effect warnings.
#pragma warning(disable: 4555) // expression has no effect; expected expression with side-effect
#endif

#if _MSC_VER == 1310
// Visual C++ 7.1 doesn't like it's own auto_ptr when returned from a function.
#pragma warning(disable: 4239) // disable nonstandard extension used (reference to non const lvalue)
#endif

#if _MSC_VER == 1400
#pragma warning(disable: 4548) // expression before comma has no effect; expected expression with side-effect
#pragma warning(disable: 4127) // warning C4127: conditional expression is constant
#pragma warning(disable: 4350) // behavior change
#pragma warning(disable: 4365) // conversion from signed to unsigned (also complains about long a = 34L;, useless warning)
#pragma warning(disable: 4626) // assignment operator could not be generated because a base class assignment operator is inaccessible
#endif

#ifdef NDEBUG
#pragma warning(disable: 4555) // expression has no effect; expected expression with side-effect
#pragma warning(disable: 4702) // code not reached (optimizer causes catch to be never reached)
#endif


#include "..\include\msfbase.h"


#define _ATL_EX_CONVERSION_MACROS_ONLY // Only use the new Unicode <-> Ansi conversion macro's.

using namespace std;
using namespace MSF;

PCWSTR const wszVVVFileRootExt     = L"VVVFile";
PCWSTR const wszVVVExtension       = L".vvv";
const TCHAR* const tszVVVExtension =_T(".vvv");

// Artificial limit to prevent allocating of to much memory. (remove if required)
const unsigned int MAX_VVV_ITEM_SIZE = 3000;
