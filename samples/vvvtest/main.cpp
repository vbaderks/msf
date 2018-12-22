//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"


#include "../include/coinitialize.h"
#include "../include/macros.h"
#include "../include/testrunner.h"

#include "module.h"
#include "testcom.h"
#include "testinfotip.h"
#include "testcolumnprovider.h"
#include "testpropsheetext.h"
#include "testshellfolder.h"
#include "testcontextmenu.h"


// The ATL test COM objects require an CAtlModule derived instancee.
CModule _Module;


// Call as:
// <progname>.exe: just execute all test and wait for key press
// <progname>.exe TestName: execute only test 'TestName' and wait for key press
// <progname>.exe TestName n: execute test 'TestName' n times and don't wait.
int _tmain(int argc, _TCHAR* argv[])
{
	const test_t tests[] =
	{
		{_T("TestCOMRegisterAndUnregister"),                TestCOM::RegisterAndUnregister},
		{_T("TestInfoTipSanity"),                           TestInfoTip::Sanity},
		{_T("TestInfoTipFileNotFound"),                     TestInfoTip::FileNotFound},
		{_T("TestInfoTipIncorrectType"),                    TestInfoTip::IncorrectType},
		{_T("TestInfoTipNotImplementedFunctions"),          TestInfoTip::NotImplementedFunctions},
		{_T("TestColumnProviderSanity"),                    TestColumnProvider::Sanity},
		{_T("TestColumnProviderGetItemDataWithUpdateItem"), TestColumnProvider::GetItemDataWithUpdateItem},
		{_T("TestColumnProviderDirectory"),                 TestColumnProvider::Directory},
		{_T("TestColumnProviderFileNotFound"),              TestColumnProvider::FileNotFound},
		{_T("TestColumnProviderIncorrectType"),             TestColumnProvider::IncorrectType},
		{_T("TestColumnProviderStandardSortPerformance"),   TestColumnProvider::StandardSortPerformance},
		{_T("TestColumnProviderSimpleSort"),                TestColumnProvider::SimpleSort},
		{_T("TestPropSheetExtSanity"),                      TestPropSheetExt::Sanity},
		{_T("TestPropSheetExtEmptyList"),                   TestPropSheetExt::EmptyList},
		{_T("TestPropSheetExtMultipleSelection"),           TestPropSheetExt::MultipleSelection},
		{_T("TestPropSheetExtFileNotFound"),                TestPropSheetExt::FileNotFound},
		{_T("TestPropSheetExtIncorrectType"),               TestPropSheetExt::IncorrectType},
		{_T("TestShellFolderSanity"),                       TestShellFolder::Sanity},
		{_T("TestShellFolderFileNotFound"),                 TestShellFolder::FileNotFound},
		{_T("TestShellFolderIncorrectType"),                TestShellFolder::IncorrectType},
		{_T("TestShellFolderFileTwice"),                    TestShellFolder::FileTwice},
		{_T("TestShellFolderGetShellFolderView"),           TestShellFolder::GetShellFolderView},
		{_T("TestShellFolderEnumObjects"),                  TestShellFolder::EnumObjects},
		{_T("TestContextMenuSanity"),                       TestContextMenu::Sanity},
		{_T("TestContextMenuEmptyList"),                    TestContextMenu::EmptyList},
		{_T("TestContextMenuMultipleSelection"),            TestContextMenu::MultipleSelection},
		{_T("TestContextMenuIncorrectType"),                TestContextMenu::IncorrectType}
	};

	CCoInitialize coinitialize;

	return CTestRunner::Main(tests, MSF_ARRAY_SIZE(tests), argc, argv);
}
