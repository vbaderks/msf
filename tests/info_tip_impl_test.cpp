//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

#include "pch.h"

#include <msf/info_tip_impl.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace msf;
using namespace ATL;
using std::wstring;


class __declspec(novtable) __declspec(uuid("E908766F-E922-40CC-994E-B677FE5237F6")) InfoTipTest :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<InfoTipTest, &__uuidof(InfoTipTest)>,
    public InfoTipImpl<InfoTipTest>
{
public:
    BEGIN_COM_MAP(InfoTipTest)
        COM_INTERFACE_ENTRY(IQueryInfo)
        COM_INTERFACE_ENTRY(IInitializeWithFile)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    void InitializeCore(const wchar_t* /*szFilename*/, DWORD /*dwMode*/) noexcept(false) override
    {
    }

    PCWSTR GetInfoTip(DWORD /* dwFlags */) noexcept(false) final
    {
        return L"";
    }

    InfoTipTest(const InfoTipTest&) = delete;
    InfoTipTest(InfoTipTest&&) = delete;
    InfoTipTest& operator=(const InfoTipTest&) = delete;
    InfoTipTest& operator=(InfoTipTest&&) = delete;

protected:
    InfoTipTest() noexcept(false) = default; // noexcept(false) needed as ATL base class is not defined noexcept.
    ~InfoTipTest() = default;
};


_COM_SMARTPTR_TYPEDEF(IInitializeWithFile, __uuidof(IInitializeWithFile));

class Module : public CAtlDllModuleT<Module>
{
};

MSF_WARNING_SUPPRESS(26426) // Global initializer calls a non-constexpr function 'Module::Module'
Module _Module;
MSF_WARNING_UNSUPPRESS()


TEST_CLASS(InfoTipImplTest)
{
public:
    TEST_METHOD(IInitializeWithFile_Initialize)
    {
        LPUNKNOWN m_pInnerUnk;
        CComCoClass<InfoTipTest>::CreateInstance(nullptr, &m_pInnerUnk);
        IInitializeWithFilePtr persistPtr(m_pInnerUnk);
        m_pInnerUnk->Release();

        HRESULT hresult = persistPtr->Initialize(L"FILEPATH", 77);
        Assert::AreEqual(S_OK, hresult);
        // TODO: extend test to verify passing of arguments.

        hresult = persistPtr->Initialize(L"FILEPATH", 77);
        Assert::AreEqual(HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED), hresult);
    }
};
