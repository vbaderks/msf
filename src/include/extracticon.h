//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#pragma once


#include "msfbase.h"
#include "pidl.h"


namespace msf
{

template <typename TItem>
class __declspec(novtable) ExtractIcon :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public IExtractIcon
{
public:
    class Icon final
    {
    public:
        explicit Icon(HICON icon = nullptr) noexcept :
            m_icon(icon)
        {
        }

        ~Icon()
        {
            dispose();
        }

        Icon(const Icon&) = delete;
        Icon(Icon&&) = delete;
        Icon& operator=(const Icon&) = delete;
        Icon& operator=(Icon&&) = delete;

        Icon& operator=(HICON icon) noexcept
        {
            dispose();
            m_icon = icon;
            return *this;
        }

        void release() noexcept
        {
            m_icon = nullptr;
        }

        HICON get() const noexcept
        {
            return m_icon;
        }

    private:
        void dispose() noexcept
        {
            if (m_icon)
            {
                ATLVERIFY(DestroyIcon(m_icon));
                m_icon = nullptr;
            }
        }

        HICON m_icon;
    };

    ExtractIcon(const ExtractIcon&) = delete;
    ExtractIcon(ExtractIcon&&) = delete;
    ExtractIcon& operator=(const ExtractIcon&) = delete;
    ExtractIcon& operator=(ExtractIcon&&) = delete;

    static ATL::CComPtr<IExtractIcon> CreateInstance(const TItem& item)
    {
        ATL::CComObject<ExtractIcon<TItem> >* instance;
        const HRESULT hr = ATL::CComObject<ExtractIcon<TItem> >::CreateInstance(&instance);
        if (FAILED(hr))
            RaiseException(hr);

        ATL::CComPtr<IExtractIcon> extractIcon(instance);
        instance->Initialize(item);
        return extractIcon;
    }

    static HICON GetIcon(HIMAGELIST himl, int i, UINT flags = 0)
    {
        const HICON icon = ImageList_GetIcon(himl, i, flags);
        RaiseExceptionIf(!icon);
        return icon;
    }

    DECLARE_NOT_AGGREGATABLE(ExtractIcon)

    BEGIN_COM_MAP(ExtractIcon)
        COM_INTERFACE_ENTRY(IExtractIcon)
    END_COM_MAP()

protected:
    ExtractIcon() noexcept : m_nIconIndex(-1), m_uFlags(0)
    {
    }

    ~ExtractIcon() = default;

    void Initialize(const TItem& item)
    {
        m_pidl.CloneFrom(item.GetItemIdList());
    }

    HRESULT __stdcall GetIconLocation(UINT uFlags, PWSTR /*szIconFile*/, UINT /*cchMax*/, _Out_ int* piIndex, _Out_ UINT* pwFlags) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ExtractIcon::GetIconLocation, instance=%p, uFlags=%x\n", this, uFlags);

        try
        {
            m_nIconIndex = TItem(m_pidl.GetRelative()).GetIconOf(uFlags);

            *piIndex = 0; // must initialize index as it is a required out parameter.
            *pwFlags = GIL_NOTFILENAME;
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall Extract(PCWSTR /*pszFile*/, UINT /*nIconIndex*/, _Out_opt_ HICON* phiconLarge, _Out_opt_ HICON* phiconSmall, UINT nIconSize) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ExtractIcon::Extract, instance=%p, pl=%p, ps=%p\n", this, phiconLarge, phiconSmall);

        try
        {
            if (m_nIconIndex == -1)
                return E_INVALIDARG; // GetIconLocation not called.

            HIMAGELIST himLarge;
            HIMAGELIST himSmall;
            RaiseExceptionIf(!Shell_GetImageLists(&himLarge, &himSmall));

            Icon iconLarge;
            if (phiconLarge)
            {
                if (LOWORD(nIconSize) != 32)
                    return E_INVALIDARG;

                iconLarge = GetIcon(himLarge, m_nIconIndex);
                *phiconLarge = iconLarge.get();
            }

            if (phiconSmall)
            {
                if (HIWORD(nIconSize) != 16)
                    return E_INVALIDARG;

                *phiconSmall = GetIcon(himSmall, m_nIconIndex);
            }

            iconLarge.release();

            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

private:
    // Member variables.
    int   m_nIconIndex;
    UINT  m_uFlags;
    ItemIDList m_pidl;
};

} // namespace msf
