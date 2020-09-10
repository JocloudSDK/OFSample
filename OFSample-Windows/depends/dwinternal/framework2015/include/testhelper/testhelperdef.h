/*
* ��������
*/

#ifdef UNIT_TEST

#pragma warning(disable:4530)

#include <windows.h>
#include <Shellapi.h>
#include <map>
#include <iostream>

#include <qglobal.h>
#include <QApplication>

#include "duifw/duiapplication.h"

#include "testchannel.h"

#ifdef BUILD_YY_TESTHELPER
#	 define YY_TESTHELPER_EXPORT	Q_DECL_EXPORT
#else
#	 define YY_TESTHELPER_EXPORT	Q_DECL_IMPORT
#endif //BUILD_YY_TESTHELPER

#define		USE_LOGIN_LOGINOUT	0

namespace testing
{
	class Environment;

	Environment* AddGlobalTestEnvironment(Environment* env);

	void InitGoogleTest(int* argc, char** argv);
	
	void InitGoogleTest(int* argc, wchar_t** argv);
}

#define	DEFINE_DEFAULT_ENVIRONMENT(TestEnvironment)					\
			class TestEnvironment : public testing::Environment		\
			{														\
			public:													\
				virtual void SetUp()								\
				{													\
					TestConfig* psConfig = TestConfig::GetConfig();	\
					if (psConfig == NULL)							\
					{												\
						printf("�����ļ������ڣ������˳�");			\
						exit(0);									\
					}												\
				}													\
				\
				virtual void TearDown()								\
				{													\
					if (!TestChannel::RunChannelTest())				\
					{												\
						printf("Channel Test Failed\n");			\
					}												\
					printf("Test Environment TearDown\n");			\
				}													\
			}

#define UNIT_TEST_RUN_ENTRY(TestEnv)							\
	int g_Argc = 0;																\
	char** g_Argv = NULL;														\
																				\
	DWORD WINAPI TestLoop(LPVOID lpParam)										\
	{																			\
		Q_UNUSED(lpParam);														\
																				\
		/* �ȴ�YY��������	*/													\
		while (qApp == NULL || (DuiApplication*)qApp == NULL)					\
		{																		\
			Sleep(20);															\
		}																		\
		Sleep(2000);															\
																				\
		std::cout << "Test Run\n";												\
		testing::AddGlobalTestEnvironment(new TestEnv);							\
																				\
		testing::InitGoogleTest(&g_Argc, g_Argv);								\
																				\
		RUN_ALL_TESTS();														\
																				\
		Sleep(5000); /*��������������ɺ�ͣ��5�룬�˳�YY */					\
		exit(0);																\
																				\
		return 0;																\
	}																		\
																				\
	/* �������DLL�����ṩ�˺�������YY����ʱ���ж��������DLL�Ҵ˺������ھ��������� */	\
	extern "C" int __declspec(dllexport) InitUnitTest(int argc, char* argv[])	\
	{																		\
		g_Argc = argc;														\
		g_Argv = argv;														\
		if (!AllocConsole())												\
		{																	\
		return 0;														\
		}																	\
		\
		FILE* fp;															\
		freopen_s(&fp, "CONOUT$","w",stdout);								\
		\
		HANDLE sHandle = CreateThread(NULL, 0, TestLoop, NULL, 0, NULL);	\
		SetThreadPriority(sHandle, THREAD_PRIORITY_ABOVE_NORMAL);			\
		\
		return 1;															\
	}
																		

#endif // UNIT_TEST