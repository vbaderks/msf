﻿//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "ole_string.h"

#include <shobjidl.h>

namespace msf {

class IItemNameLimitsImplDefault
{
public:
    // Purpose: Override this function to control which chars are valid.
    static const wchar_t* GetValidChars() noexcept
    {
        return nullptr;
    }

    // Purpose: Override this function to control which chars are invalid.
    static const wchar_t* GetInvalidChars() noexcept
    {
        return nullptr;
    }
};

template <typename T, typename TItem,
          typename TItemV = IItemNameLimitsImplDefault, typename TItemI = IItemNameLimitsImplDefault>
class __declspec(novtable) IItemNameLimitsImpl :
    public IItemNameLimits
{
public:
    IItemNameLimitsImpl() = default;

    IItemNameLimitsImpl(const IItemNameLimitsImpl&) = delete;
    IItemNameLimitsImpl(IItemNameLimitsImpl&&) = delete;
    IItemNameLimitsImpl& operator=(const IItemNameLimitsImpl&) = delete;
    IItemNameLimitsImpl& operator=(IItemNameLimitsImpl&&) = delete;

    // IItemNameLimits
    HRESULT __stdcall GetValidCharacters(PWSTR* ppwszValidChars, PWSTR* ppwszInvalidChars) override
    {
        try
        {
            ATLTRACE(L"IItemNameLimitsImpl::GetValidCharacters\n");

            const wchar_t* pszValidChars = TItemV::GetValidChars();
            if (!pszValidChars)
            {
                *ppwszValidChars = nullptr;

                const wchar_t* pszInvalidChars = TItemI::GetInvalidChars();
                if (!pszInvalidChars)
                {
                    *ppwszInvalidChars = nullptr;
                }
                else
                {
                    *ppwszInvalidChars = OleString::Dup(pszInvalidChars);
                }
            }
            else
            {
                *ppwszValidChars   = OleString::Dup(pszValidChars);
                *ppwszInvalidChars = nullptr;
            }
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // Purpose: called by the shell to retrieve the max length.
    //          Length can depend on the current string. (UNC pathname has
    //          different length then local path, etc).
    HRESULT __stdcall GetMaxLength(__RPC__in_string PCWSTR name, __RPC__out int* maxNameLength) override
    {
        try
        {
            ATLTRACE(L"IItemNameLimitsImpl::GetMaxLength, name=%s\n", name);

            // The TItem must implement the static function GetMaxItemNameLength.
            *maxNameLength = TItem::GetMaxNameLength(name);

            ATLASSERT(*maxNameLength >= 0 && "negative length?");
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

protected:
    ~IItemNameLimitsImpl() = default;
 };

} // namespace msf
