//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "smartptr/dataobjectptr.h"
#include "stgmedium.h"
#include "formatetc.h"

namespace MSF
{

/// <summary>Support class to handle the CF_HDROP format.</summary>
/// <remarks>
/// The CF_HDROP format is used by the shell to transfer a group of existing files.
/// The handle refers to a set of DROPFILES structures.
/// </remarks>
class CCfHDrop
{
public:
    /// <summary>Returns true when the data object has the CF_HDROP format.</summary>
    static bool IsFormat(_In_ IDataObject* pdataobject)
    {
        CFormatEtc formatetc(CF_HDROP);
        return SUCCEEDED(pdataobject->QueryGetData(&formatetc));
    }


    explicit CCfHDrop(IDataObjectPtr dataobject)
    {
        dataobject.GetData(CFormatEtc(CF_HDROP), m_stgmedium);
    }


    bool IsEmpty() const noexcept
    {
        return GetFileCount() == 0;
    }


    unsigned int GetFileCount() const noexcept
    {
        ATLASSERT(m_stgmedium.tymed == TYMED_HGLOBAL && "Unable to retrieve filecount");
        return ::DragQueryFile(static_cast<HDROP>(m_stgmedium.hGlobal), static_cast<UINT>(-1), nullptr, 0);
    }


    CString GetFile(unsigned int iFile) const
    {
        ATLASSERT(iFile < GetFileCount() && "Index out of bounds");

        wchar_t szFileName[MAX_PATH];
        if (!::DragQueryFile(reinterpret_cast<HDROP>(m_stgmedium.hGlobal), iFile, szFileName,
                            MSF_ARRAY_SIZE(szFileName)))
            throw _com_error(HRESULT_FROM_WIN32(GetLastError()));

        return szFileName;
    }

private:

    CStgMedium m_stgmedium;
};

} // namespace MSF
