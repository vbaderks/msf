//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
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

// As MSF targets VC 7.0, 7.1 and 8.0 it cannot use the CRT secure versions of 8.0
// as these functions are not available on 7.0 and 7.1
#define _CRT_SECURE_NO_DEPRECATE


// Disable not usefull Wall warnings
#pragma warning(disable: 4061) // enumerate 'xxx' in switch of enum 'yyy' is not explicitly handled by a case label
#pragma warning(disable: 4820) // 'x' bytes padding added after member.
#pragma warning(disable: 4710) // function not inlined.
#pragma warning(disable: 4514) // unreferenced inline function has been removed.
#pragma warning(disable: 4711) // function xxx is selected for automatic inline expansion.
#pragma warning(disable: 4265) // class has virtual functions, but destructor is not virtual
#pragma warning(disable: 4505) // unreferenced local function has been removed.
#pragma warning(disable: 4928) // illegal copy-initialization; more than one user-defined conversion has been implicitly applied

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


// Standard C++ library
#pragma warning(push)

#if _MSC_VER < 1400
#pragma warning(disable: 4217) // member template functions cannot be used for copy-assignment or copy-construction
#endif

#pragma warning(disable: 4619) // #pragma warning : there is no warning number 'xxx'

#include <vector>
#include <algorithm>
#include <map>

#pragma warning(pop)


// Win32
#pragma warning(push)
#pragma warning(disable: 4668) // not a preprocessor macro
#pragma warning(disable: 4917) // a GUID can only be defines to a class, interface, etc.
#pragma warning(disable: 4191) // 'type cast' : unsafe conversion from 'FARPROC' to 'PFN'

#if _MSC_VER == 1400
#pragma warning(disable: 4826) // Conversion from 'const void *' to 'void *' is sign-extended. This may cause unexpected runtime behavior.
#endif

#include <shlobj.h>
#include <shobjidl.h>

#pragma warning(pop)


// Visual C++ compiler support includes (depends on Win32 defines COM interfaces)
#pragma warning(push)
#pragma warning(disable: 4668) // not a preprocessor macro
#pragma warning(disable: 4917) // a GUID can only be defines to a class, interface, etc.

#if _MSC_VER < 1400
#pragma warning(disable: 4217) // member template functions cannot be used for copy-assignment or copy-construction
#endif

#include <comdef.h>

#pragma warning(pop)


// ATL
#pragma warning(push)
#pragma warning(disable: 4263) // 'function' : member function does not override any base class virtual member function
#pragma warning(disable: 4264) // auto generated after C4263
#pragma warning(disable: 4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable: 4100) // unreferenced formal parameter
#pragma warning(disable: 4127) // conditional expression is constant
#pragma warning(disable: 4512) // assignment operator could not be generated

#if _MSC_VER == 1300 // VS.Net 2002 requires some extra warnings to be disabled.
#pragma warning(disable: 4191) // 'type cast' : unsafe conversion from 'FARPROC' to 'DLLGETVERSIONPROC'
#pragma warning(disable: 4217) // member template functions cannot be used for copy-assignment or copy-construction
#pragma warning(disable: 4302) // 'type cast' : truncation from 'LPSTR' to 'TCHAR'
#pragma warning(disable: 4529) // forming a pointer-to-member requires explicit use of the address-of operator ('&') and a qualified name
#pragma warning(disable: 4555) // expression has no effect; expected expression with side-effect
#pragma warning(disable: 4619) // #pragma warning : there is no warning number 'xxx'
#pragma warning(disable: 4640) // construction of local static object is not thread-safe
#endif

#if _MSC_VER == 1310 && defined(NDEBUG)
#pragma warning(disable: 4548) //  warning C4548: expression before comma has no effect; expected expression with side-effect
#endif

#if _MSC_VER == 1400
#pragma warning(disable: 4619) // #pragma warning : there is no warning number 'xxx'
#endif

#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <atlsnap.h>

#pragma warning(pop)


#if _MSC_VER == 1300 // ATL 7.0 BEGIN_MSG_MAP generates: expression no effect warnings.
#pragma warning(push)
#pragma warning(disable: 4555) // expression has no effect; expected expression with side-effect
#endif


#define _ATL_EX_CONVERSION_MACROS_ONLY // Only use the new Unicode <-> Ansi conversion macro's.

using namespace std;

namespace MSF {}
using namespace MSF;
