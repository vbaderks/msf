//
// (C) Copyright by Victor Derks
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

// Allow use of features specific to IE.
#define _WIN32_IE 0x0501  // Target IE 5.01 (required for definition of shell interfaces)

#define _ATL_ALL_WARNINGS // turns off ATL's hiding of some common and often safely ignored warning messages.

// To support testing with the /Wall switch certain warning must be disabled when including
// Win32, C\C++ runtime or ATL header files. Some warning must be kept disabled as templates will
// only generate the warning when they are actual used.
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

#if _MSC_VER == 1500
#pragma warning(disable: 4548) // expression before comma has no effect; expected expression with side-effect
#endif

#include "../include/msfbase.h"

#pragma warning(push)

#if _MSC_VER == 1500
#pragma warning(disable: 4365) // conversion from signed to unsigned (also complains about long a = 34L;, useless warning)
#endif

#ifdef _PREFAST_ // defined for the static analyser in VC 2005 / 2008
#pragma warning(disable: 6011) // Dereferencing NULL pointer.
#pragma warning(disable: 6385) // Invalid data.....
#pragma warning(disable: 6386) // Buffer overrun: accessing 'argument 1'...'
#endif

#include <tchar.h>
#include <atlctl.h>

#pragma warning(pop)

using namespace std;
using namespace MSF;
