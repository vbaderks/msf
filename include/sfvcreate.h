//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


namespace MSF
{

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
