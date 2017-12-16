//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../include/infotipimpl.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MSF;
using std::wstring;

namespace test
{
    class DECLSPEC_UUID("EDD37CEF-F1E0-42bb-9AEF-177E0306AA71") CInfoTipTest;

    class ATL_NO_VTABLE CInfoTipTest :
        public CComObjectRootEx<CComSingleThreadModel>,
        public CComCoClass<CInfoTipTest, &__uuidof(CInfoTipTest)>,
        public InfoTipImpl<CInfoTipTest>
    {
    public:
        BEGIN_COM_MAP(CInfoTipTest)
            COM_INTERFACE_ENTRY(IQueryInfo)
            COM_INTERFACE_ENTRY(IInitializeWithFile)
        END_COM_MAP()

        DECLARE_PROTECT_FINAL_CONSTRUCT()

        void InitializeCore(const wchar_t* szFilename, DWORD /*dwMode*/) override
        {
        }

        wstring GetInfoTip(DWORD /* dwFlags */) override
        {
            return wstring();
        }
    };


    _COM_SMARTPTR_TYPEDEF(IInitializeWithFile, __uuidof(IInitializeWithFile));

    class CModule : public CAtlDllModuleT< CModule >
    {
    };

    CModule _Module;


    TEST_CLASS(InfoTipImplTest)
    {
    public:
        
        TEST_METHOD(IInitializeWithFile_Initialize)
        {
            LPUNKNOWN m_pInnerUnk;
            CComCoClass<CInfoTipTest>::CreateInstance(nullptr, &m_pInnerUnk);
            IInitializeWithFilePtr persistPtr(m_pInnerUnk);
            m_pInnerUnk->Release();

            HRESULT hresult = persistPtr->Initialize(L"FILEPATH", 77);
            Assert::AreEqual(S_OK, hresult);
            // TODO: extend test to verify passing of arguments.

            hresult = persistPtr->Initialize(L"FILEPATH", 77);
            Assert::AreEqual(HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED), hresult);
        }
    };
}
