//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace MSF
{

class CContextCommand
{
public:
    virtual ~CContextCommand()
    {
    }


    virtual void operator()(const CMINVOKECOMMANDINFO* /* pici */,
                            const std::vector<ATL::CString>& /* filenames */)
    {
    }
};

} // end namespace MSF
