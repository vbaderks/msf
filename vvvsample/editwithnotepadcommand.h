//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../include/contextcommand.h"
#include "../include/util.h"


class CEditWithNotepadCommand : public CContextCommand
{
public:
	virtual void operator()(const CMINVOKECOMMANDINFO* /* pici */,
	                        const std::vector<CString>& filenames)
	{
		ATLASSERT(filenames.size() == 1); // can only handle 1 file.

		// Use the command line param to pass the exe filename. This causes
		// Windows to use the path to find notepad.
		CString strCmd = _T("notepad.exe \"") + filenames[0] + _T("\"");

		CreateProcess(NULL, strCmd.GetBuffer());
	}
};
