//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include <shobjidl.h>
#include "olestring.h"


// Visual C++ doesn't like exception handlers that are optimized away in release mode.
#if _MSC_VER == 1310 && defined(NDEBUG)
#pragma warning(push)
#pragma warning(disable: 4702) // disable unreachable code
#endif


namespace MSF
{

class IItemNameLimitsImplDefault
{
public:
    // Purpose: Override this function to control which chars are valid.
    static const TCHAR* GetValidChars()
    {
        return NULL;
    }


    // Purpose: Override this function to control which chars are invalid.
    static const TCHAR* GetInvalidChars()
    {
        return NULL;
    }
};

template <typename T, typename TItem,
          typename TItemV = IItemNameLimitsImplDefault, typename TItemI = IItemNameLimitsImplDefault>
class ATL_NO_VTABLE IItemNameLimitsImpl :
    public IItemNameLimits
{
public:
    // IItemNameLimits
    STDMETHOD(GetValidCharacters)(LPWSTR* ppwszValidChars, LPWSTR* ppwszInvalidChars)
    {
        try
        {
            ATLTRACE2(atlTraceCOM, 0, _T("IItemNameLimitsImpl::GetValidCharacters\n"));

            const TCHAR* pszValidChars = TItemV::GetValidChars();
            if (pszValidChars == NULL)
            {
                *ppwszValidChars = NULL;

                const TCHAR* pszInvalidChars = TItemI::GetInvalidChars();
                if (pszInvalidChars == NULL)
                {
                    *ppwszInvalidChars = NULL;
                }
                else
                {
                    *ppwszInvalidChars = COleString::Dup(pszInvalidChars);
                }
            }
            else
            {
                *ppwszValidChars   = COleString::Dup(pszValidChars);
                *ppwszInvalidChars = NULL;
            }
            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }


    // Purpose: called by the shell to retrieve the max length.
    //          Lenght can depend on the current string. (UNC pathname has
    //          different length then local path, etc).
    //
    STDMETHOD(GetMaxLength)(__RPC__in_string LPCWSTR pszName, __RPC__out int* piMaxNameLen)
    {
        try
        {
            ATLTRACE2(atlTraceCOM, 0, _T("IItemNameLimitsImpl::GetMaxLength, name=%s\n"), pszName);

            // The TItem must implement the static function GetMaxItemNameLength.
            *piMaxNameLen = TItem::GetMaxNameLength(pszName);

            ATLASSERT(*piMaxNameLen >= 0 && "negative length?");
            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }
 };

} // namespace MSF

#if _MSC_VER == 1310 && defined(NDEBUG)
#pragma warning(pop)
#endif
