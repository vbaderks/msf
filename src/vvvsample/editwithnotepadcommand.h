//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include <msf.h>


class CEditWithNotepadCommand : public CContextCommand
{
public:
    void operator()(const CMINVOKECOMMANDINFO* /* pici */, const std::vector<CString>& filenames) override
    {
        ATLASSERT(filenames.size() == 1); // can only handle 1 file.

        // Use the command line parameters to pass the exe filename. This causes
        // Windows to use the path to find notepad.
        CString strCmd = L"notepad.exe \"" + filenames[0] + L"\"";

        CreateProcess(nullptr, strCmd.GetBuffer());
    }
};
