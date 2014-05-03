//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


// Purpose: base include file for msf header files. Includes most common header files.
// MSF supports the following compilers:
// _MSC_VER = 1700 : Visual Studio 2012
// _MSC_VER = 1800 : Visual Studio 2013
// _PREFAST_ is defined when the Visual Studio C\C++ compiler is executed with the /analyze switch, it activates the static analyzer.

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

// The Windows SDK headers files and ATL header files generate static analyzer warnings. Filter these warnings.
#ifdef _PREFAST_ // 
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
//#define _Out_writes_to_(size,count)
#endif

#ifndef _Outptr_opt_
//#define _Outptr_opt_
#endif

#ifdef _PREFAST_ // defined when Visual Studio C\C++ /analyze is used.
#pragma warning(disable: 6509)  // Invalid annotation: 'return' cannot be referenced in some contexts. [must be disabled globaly to supress issues in Win SDK]
#endif

// core MSF include files
#include "shlobjhidden.h"
#include "macros.h"
