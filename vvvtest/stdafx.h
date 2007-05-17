//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//

// Enabled improved type checking of the Win32 SDK header files.
#define STRICT

// Allow use of features specific to Windows NT 4 or later.
// Change this to the appropriate value to target Windows 2000 or later.
#define _WIN32_WINNT 0x0400

// Allow use of features specific to Windows 98 or later.
// Change this to the appropriate value to target Windows Me or later.
#define _WIN32_WINDOWS 0x0410


// Disable not usefull Wall warnings
#pragma warning(disable: 4265) // class has virtual functions, but destructor is not virtual
#pragma warning(disable: 4505) // unreferenced local function has been removed.
#pragma warning(disable: 4514) // unreferenced inline function has been removed.
#pragma warning(disable: 4710) // function not inlined.
#pragma warning(disable: 4711) // function xxx is selected for automatic inline expansion.
#pragma warning(disable: 4820) // 'x' bytes padding added after member.

#if _MSC_VER == 1300 // VS.Net 2002 requires some extra warnings to be disabled.
#pragma warning(disable: 4555) // expression has no effect; expected expression with side-effect
#endif

#if _MSC_VER == 1400
#pragma warning(disable: 4365) // conversion from signed to unsigned (also complains about long a = 34L;, useless warning)
#pragma warning(disable: 4555) // expression has no effect; expected expression with side-effect
#endif

#include "../include/msfbase.h"

#include <tchar.h>
#include <atlctl.h>

using namespace std;
using namespace MSF;
