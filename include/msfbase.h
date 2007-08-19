//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


// Purpose: base include file for msf header files. Includes most common header files.


// As MSF targets VC 7.0, 7.1 and 8.0 it cannot use the CRT secure versions of 8.0
// as these functions are not available on 7.0 and 7.1. Disable the warning.
#define _CRT_SECURE_NO_DEPRECATE


// Win32
#pragma warning(push)
#pragma warning(disable: 4668) // not a preprocessor macro
#pragma warning(disable: 4917) // a GUID can only be defines to a class, interface, etc.
#pragma warning(disable: 4191) // 'type cast' : unsafe conversion from 'FARPROC' to 'PFN'

#if _MSC_VER == 1400
#pragma warning(disable: 4619) // #pragma warning : there is no warning number 'xxx'
#pragma warning(disable: 4826) // Conversion from 'const void *' to 'void *' is sign-extended. This may cause unexpected runtime behavior.
#endif

#ifdef _PREFAST_
#pragma warning(disable: 6387) // 'return value' might be '0': this does not adhere to the specification for the function
#endif

#include <shlobj.h>
#include <shobjidl.h>

#pragma warning(pop)


// Visual C++ compiler support includes.
#pragma warning(push)
#pragma warning(disable: 4668) // not a preprocessor macro
#pragma warning(disable: 4917) // a GUID can only be defines to a class, interface, etc.

#if _MSC_VER < 1400
#pragma warning(disable: 4217) // member template functions cannot be used for copy-assignment or copy-construction
#endif

#ifdef _PREFAST_
#pragma warning(disable: 6386) // Buffer overrun: accessing 'argument 1'...'
#endif

// Note: comdef.h will create only smartpointers for defined interfaces. Include all required
//       Win32 header files before including comdef.h
#include <comdef.h>

#pragma warning(pop)


// Standard ATL include files.
#pragma warning(push)
#pragma warning(disable: 4263) // 'function' : member function does not override any base class virtual member function
#pragma warning(disable: 4264) // auto generated after C4263
#pragma warning(disable: 4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable: 4100) // unreferenced formal parameter
#pragma warning(disable: 4127) // conditional expression is constant
#pragma warning(disable: 4512) // assignment operator could not be generated
#pragma warning(disable: 4619) // #pragma warning : there is no warning number 'xxx'

#if _MSC_VER == 1300
#pragma warning(disable: 4191) // 'type cast' : unsafe conversion from 'FARPROC' to 'DLLGETVERSIONPROC'
#pragma warning(disable: 4217) // member template functions cannot be used for copy-assignment or copy-construction
#pragma warning(disable: 4302) // 'type cast' : truncation from 'LPSTR' to 'TCHAR'
#pragma warning(disable: 4529) // forming a pointer-to-member requires explicit use of the address-of operator ('&') and a qualified name
#pragma warning(disable: 4555) // expression has no effect; expected expression with side-effect
#pragma warning(disable: 4640) // construction of local static object is not thread-safe
#endif

#if _MSC_VER == 1310 && defined(NDEBUG)
#pragma warning(disable: 4548) //  warning C4548: expression before comma has no effect; expected expression with side-effect
#endif

#ifdef _PREFAST_ // defined for the static analyser in VC 2005
#pragma warning(disable: 6386) // Buffer overrun: 
#endif

#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <atlsnap.h>

#pragma warning(pop)


// Standard C++ library
#pragma warning(push)

#pragma warning(disable: 4619) // #pragma warning : there is no warning number 'xxx'

#if _MSC_VER < 1400
#pragma warning(disable: 4217) // member template functions cannot be used for copy-assignment or copy-construction
#endif

#if _MSC_VER == 1400
#pragma warning(disable: 4548) // expression before comma has no effect; expected expression with side-effect
#endif

#include <vector>
#include <algorithm>

#pragma warning(pop)


// core MSF include files
#include "shlobjhidden.h"
#include "macros.h"
