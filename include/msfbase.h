//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


// Purpose: base include file for msf header files. Includes most common header files.


// As MSF targets VC 7.0, 7.1, 8.0 and 9.0 it cannot use the CRT secure versions of 8.0 or 9.0
// as these functions are not available on 7.0 and 7.1. Disable the warning.
#define _CRT_SECURE_NO_DEPRECATE

// To support testing with the /Wall switch certain warning must be disabled when including
// Win32, C\C++ runtime or ATL header files.

#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter
#pragma warning(disable: 4127) // conditional expression is constant
#pragma warning(disable: 4191) // 'type cast' : unsafe conversion from 'FARPROC' to 'PFN'
#pragma warning(disable: 4263) // 'function' : member function does not override any base class virtual member function
#pragma warning(disable: 4264) // auto generated after C4263
#pragma warning(disable: 4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable: 4512) // assignment operator could not be generated
#pragma warning(disable: 4619) // #pragma warning : there is no warning number 'xxx'
#pragma warning(disable: 4668) // not a preprocessor macro
#pragma warning(disable: 4917) // a GUID can only be defines to a class, interface, etc.

#if _MSC_VER == 1300
#pragma warning(disable: 4217) // member template functions cannot be used for copy-assignment or copy-construction
#pragma warning(disable: 4302) // 'type cast' : truncation from 'LPSTR' to 'TCHAR'
#pragma warning(disable: 4529) // forming a pointer-to-member requires explicit use of the address-of operator ('&') and a qualified name
#pragma warning(disable: 4555) // expression has no effect; expected expression with side-effect
#pragma warning(disable: 4640) // construction of local static object is not thread-safe
#endif

#if _MSC_VER == 1310 && defined(NDEBUG)
#pragma warning(disable: 4548) //  warning C4548: expression before comma has no effect; expected expression with side-effect
#endif

#if _MSC_VER < 1400
#pragma warning(disable: 4217) // member template functions cannot be used for copy-assignment or copy-construction
#endif

#if _MSC_VER == 1400
#pragma warning(disable: 4548) // expression before comma has no effect; expected expression with side-effect
#pragma warning(disable: 4619) // #pragma warning : there is no warning number 'xxx'
#pragma warning(disable: 4826) // Conversion from 'const void *' to 'void *' is sign-extended. This may cause unexpected runtime behavior.
#endif

#if _MSC_VER == 1500
#pragma warning(disable: 4365) // conversion from signed to unsigned (also complains about long a = 34L;, useless warning)
#pragma warning(disable: 4548) // expression before comma has no effect; expected expression with side-effect
#endif

#if _MSC_VER < 1700
#define _Outptr_ __deref_out // Prefast defintion not available before VS 11.
#endif

// The Windows SDK headers files and ATL header files generate static analyzer warnings. Filter these warnings.
#ifdef _PREFAST_ // defined when Visual Studio C\C++ /analyze is used.
#pragma warning(disable: 6054)  // String 'Buf' might not be zero-terminated.
#pragma warning(disable: 6385)  // Invalid data.....
#pragma warning(disable: 6386)  // Buffer overrun: accessing 'argument 1'...'
#pragma warning(disable: 6387)  // 'return value' might be '0': this does not adhere to the specification for the function
#pragma warning(disable: 6388)  // '*xxxx' might not be '0':
#pragma warning(disable: 28196) // The requirement that '...' is not satisfied.
#pragma warning(disable: 28204)  // 'QueryInterface' : Only one of this override and the one at.. (caused by END_COM_MAP)
#endif

// Standard C++ library
#include <vector>
#include <algorithm>

// Win32 include files.
#include <shlobj.h>
#include <shobjidl.h>

// Note: comdef.h will create only smartpointers for defined interfaces. Include all required
//       Win32 header files before including comdef.h
#include <comdef.h>

// Standard ATL include files.
#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <atlsnap.h>

#pragma warning(pop)

// Visual Studio 2012 contains SAL2 (MS Static Analyzer Standard Annatation Language). Define missing used SAL2 macros to support older VS compilers.
#ifndef _Out_writes_to_
#define _Out_writes_to_(size,count)
#endif

#ifndef _Outptr_opt_
#define _Outptr_opt_
#endif

#ifdef _PREFAST_ // defined when Visual Studio C\C++ /analyze is used.
#pragma warning(disable: 6509)  // Invalid annotation: 'return' cannot be referenced in some contexts. [must be disabled globaly to supress issues in Win SDK]
#endif

// core MSF include files
#include "shlobjhidden.h"
#include "macros.h"
