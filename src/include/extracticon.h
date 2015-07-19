//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "pidl.h"


namespace MSF
{

template <typename TItem>
class ATL_NO_VTABLE CExtractIcon :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IExtractIcon
{
public:

    class CIcon
    {
    public:
        explicit CIcon(HICON hicon = nullptr) :
            m_hicon(hicon)
        {
        }


        ~CIcon() MSF_NOEXCEPT
        {
            dispose();
        }


        HICON operator=(HICON hicon) MSF_NOEXCEPT
        {
            dispose();
            m_hicon = hicon;
            return m_hicon;
        }


        void release() MSF_NOEXCEPT
        {
            m_hicon = nullptr;
        }


        HICON get() const MSF_NOEXCEPT
        {
            return m_hicon;
        }

    private:

        void dispose() MSF_NOEXCEPT
        {
            if (m_hicon)
            {
                ATLVERIFY(DestroyIcon(m_hicon));
                m_hicon = nullptr;
            }
        }

        HICON m_hicon;
    };


    static CComPtr<IExtractIcon> CreateInstance(const TItem& item)
    {
        CComObject<CExtractIcon<TItem> >* pinstance;
        HRESULT hr = CComObject<CExtractIcon<TItem> >::CreateInstance(&pinstance);
        if (FAILED(hr))
            RaiseException(hr);

        CComPtr<IExtractIcon> extracticon(pinstance);

        pinstance->Initialize(item);

        return extracticon;
    }


    static HICON GetIcon(HIMAGELIST himl, int i, UINT flags = 0)
    {
        HICON hicon = ImageList_GetIcon(himl, i, flags);
        RaiseExceptionIf(!hicon);
        return hicon;
    }


    DECLARE_NOT_AGGREGATABLE(CExtractIcon)

    BEGIN_COM_MAP(CExtractIcon)
        COM_INTERFACE_ENTRY(IExtractIcon)
    END_COM_MAP()


    void Initialize(const TItem& item)
    {
        m_nIconIndex = -1;
        m_pidl.CloneFrom(item.GetItemIdList());
    }


    STDMETHOD(GetIconLocation)(UINT uFlags, LPTSTR /*szIconFile*/, UINT /*cchMax*/, _Out_ int* piIndex, _Out_ UINT* pwFlags)
    {
        ATLTRACE2(atlTraceCOM, 0, L"CExtractIcon::GetIconLocation, instance=%p, uFlags=%x\n", this, uFlags);

        try
        {
            m_nIconIndex = TItem(m_pidl).GetIconOf(uFlags);

            *piIndex = 0; // must initialize index as it is a required out parameter.
            *pwFlags = GIL_NOTFILENAME;
            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }


    STDMETHOD(Extract)(LPCTSTR /*pszFile*/, UINT /*nIconIndex*/, _Out_opt_ HICON* phiconLarge, _Out_opt_ HICON* phiconSmall, UINT nIconSize)
    {
        ATLTRACE2(atlTraceCOM, 0, L"CExtractIcon::Extract, instance=%p, pl=%p, ps=%p\n", this, phiconLarge, phiconSmall);

        try
        {
            if (m_nIconIndex == -1)
                return E_INVALIDARG; // GetIconLocation not called.

            HIMAGELIST himLarge;
            HIMAGELIST himSmall;
            RaiseExceptionIf(!Shell_GetImageLists(&himLarge, &himSmall));

            CIcon iconLarge;
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
        MSF_COM_CATCH_HANDLER()
    }

private:

    // Member variables.
    int   m_nIconIndex;
    UINT  m_uFlags;
    CPidl m_pidl;
};

} // namespace MSF
