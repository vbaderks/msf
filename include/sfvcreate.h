//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

namespace MSF
{
/// <summary>Smart struct class that will initialise SDK SFV_CREATE structure.</summary>
class CSfvCreate : public SFV_CREATE
{
public:
    CSfvCreate(IShellFolder* pShf, IShellFolderViewCB* pSfvcb, IShellView* pSvOuter = NULL)
    {
        cbSize   = sizeof(SFV_CREATE);
        pshf     = pShf;
        psvOuter = pSvOuter;
        psfvcb   = pSfvcb;
    }
};

} // namespace MSF
