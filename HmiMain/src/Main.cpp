#ifdef __linux
#include "client/linux/handler/minidump_descriptor.h"
#include "client/linux/handler/exception_handler.h"
#else
#include <windows.h>
#include <direct.h>
#include <tlhelp32.h> 
#include "client/windows/handler/exception_handler.h"
#endif
#include <iostream>
#include <fstream>
#include "CompileInfo.h"
#include "Main.h"
#include "HmiCore.h"
#include "RunEnv.h"
#include "HMIProject.h"
#include "Param.h"
#include "Logger.h"
#include "CodeFormatUtility.h"
#include "IResourceService.h"
static Core::HmiCore core;

#ifdef WIN32
bool dumpCallback(const wchar_t *dump_path, const wchar_t *id,
	void *context, EXCEPTION_POINTERS *exinfo,
	MDRawAssertionInfo *assertion,
	bool succeeded)
{

	if (succeeded) {
		printf("dump guid is %ws\n", id);
	}
	else {
		printf("dump failed\n");
	}
	system("pause");
	return succeeded;
}
void InitBreakpad()
{
	google_breakpad::ExceptionHandler *pCrashHandler =
		new google_breakpad::ExceptionHandler(L".",
			NULL,
			dumpCallback,
			NULL,
			google_breakpad::ExceptionHandler::HANDLER_ALL);
}
#else

static bool dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
	void* context,
	bool succeeded)
{
	char buf[256];
	std::string path(descriptor.path());
	int pos = path.rfind('/');
	std::string name = path.substr(pos + 1, path.size() - pos - 1);
	sprintf(buf, "rm -rf `ls /data/dmp/ds/*.dmp | grep -v \"%s\"`", name.c_str());
	system(buf);
	printf("Dump path: %s\n", descriptor.path());
	return succeeded;
}
void InitBreakpad()
{
	google_breakpad::MinidumpDescriptor descriptor(".");
	google_breakpad::ExceptionHandler* eh = new google_breakpad::ExceptionHandler(descriptor,
		NULL,
		dumpCallback,
		NULL,
		true,
		-1);
}
#endif
bool TestIsExist(string filepath)
{
	if (FILE *file = fopen(filepath.c_str(), "r"))
	{
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}
int OPEN_DATASERVER()
{

#ifdef WIN32
	STARTUPINFO si = { sizeof(si) };
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION PI;
	//TCHAR szCommandLine[] = TEXT("HmiDataServer.exe -f Hmi.bin -d Run");
	TCHAR szCommandLine[] = TEXT("HmiDataServer.exe" );
	if (CreateProcess(NULL, szCommandLine, NULL, NULL,
		FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &PI))
	{
		return 1;
	}
	else
#endif
		return 1;
}
#ifdef WIN32
#define DS_EVENT "XINJE_EVENT_8452941-7892-5523-5671-AA-DATASERVER"
#define UI_EVENT "XINJE_EVENT_8452941-7892-5523-5671-AA-UIMAIN"
//HANDLE dsEvent;
HANDLE uiEvent;
int UIMainEvent()
{
	//dsEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, DS_EVENT);
	uiEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, UI_EVENT);
	if (uiEvent == NULL)
	{
		OutputDebugStringA("Open Event Failed Go Create\n");
		uiEvent = CreateEvent(NULL, FALSE, FALSE, UI_EVENT);
		if (uiEvent == NULL)
		{
			OutputDebugStringA("CreateEvent FAILED , m_hEncEvent == NULL!\n");
			return -1;
		}
		OutputDebugStringA("Create Event SUCCEED\n");
	}
	
	return 0;
}
#endif
void CLOSE_DATASERVER()
{
#ifdef WIN32

#if 1
	UIMainEvent();
	//判断当前有没有uimian进程，有的话就
	if (uiEvent!=NULL)
	{
		if (/*not has uimain process*/true)
		{
			SetEvent(uiEvent);
			ResetEvent(uiEvent);
		}
		CloseHandle(uiEvent);
	}
	
	//HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, DS_EVENT);
	//if (hEvent == NULL)
	//{
	//	/*OutputDebugStringA("Open Event Failed Go Create\n");
	//	hEvent = CreateEvent(NULL, TRUE, FALSE, MYEVENT);
	//	if (hEvent == NULL)
	//	{
	//		OutputDebugStringA("CreateEvent FAILED , m_hEncEvent == NULL!\n");
	//		return;
	//	}
	//	OutputDebugStringA("Create Event SUCCEED\n");*/
	//	
	//}
	//else
	//{
	//	//ResetEvent(hEvent);
	//	SetEvent(hEvent);
	//}
	//	
	//CloseHandle(hEvent);
#else

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {
		if (strcmp(pe.szExeFile, "HmiDataServer.exe") == 0) {
			CloseHandle(hSnapshot);
			//pid = pe.th32ProcessID;
			//CloseHandle(hSnapshot);
			HANDLE handle = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
			TerminateProcess(handle, 0);
			WaitForSingleObject(handle, INFINITE);
		}
	}
#endif
#endif
}
static void ChangeDbFilePathCode()
{
	if (!UI::CodeFormatUtility::IsStrUtf8(RunEnv::Cnf.AlarmPath.c_str()))
		UI::IResourceService::GB2312toUtf8(RunEnv::Cnf.AlarmPath);
	//if (!UI::CodeFormatUtility::IsStrUtf8(RunEnv::Cnf.SamplePath.c_str()))
		UI::IResourceService::GB2312toUtf8(RunEnv::Cnf.SamplePath);
	if (!UI::CodeFormatUtility::IsStrUtf8(RunEnv::Cnf.OperationPath.c_str()))
		UI::IResourceService::GB2312toUtf8(RunEnv::Cnf.OperationPath);
	if (!UI::CodeFormatUtility::IsStrUtf8(RunEnv::Cnf.RecipePath.c_str()))
		UI::IResourceService::GB2312toUtf8(RunEnv::Cnf.RecipePath);
}
static void LoadParam(Param& param, int argc, char ** argv)
{
	
	RunEnv::Init();
	ChangeDbFilePathCode();
	param.ParseParam(argc, argv);
	Logger::SetFlag(true);
	Logger::Ins().Init(RunEnv::Cnf.LogCnfPath);
}
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup")
#define xTEST
int main(int argc, char ** argv) {
	InitBreakpad();
	

	Param param;	
	LoadParam(param, argc, argv);
	LOG_INFO("Start HmiMain.exe\n");
	//CLOSE_DATASERVER();
//#ifdef _MSC_VER
//	if (dsEvent==NULL&&!OPEN_DATASERVER())
//		return 0;
//#endif
#ifndef TEST
	if (!RunEnv::Cnf.BinPath.empty())
	{
		//判断文件是否存在
		if (TestIsExist(RunEnv::Cnf.BinPath))
		{				 
			core.Init();
			UI::UIManager::Ins().AddExit([] {
				CLOSE_DATASERVER();
			});
		    return core.Run();
		}
		else
		{
			LOG_ERROR("Can't Open Hmi.bin: %s\n", RunEnv::Cnf.BinPath.c_str());
			return 0;
		}
	}
	else
	{
		param.PrintUsage(argv[0]);
		return 0;
	}
#else
	char buf[256];
#ifdef WIN32
	_getcwd(buf, sizeof(buf));
#else
	getcwd(buf, sizeof(buf));
#endif
	strcat(buf, "C:\\Users\\PC\\source\\repos\\Hmi\\Hmi\\bin\\HmiTest\\HMI\\Hmi.bin");

	/*int* trydmp = new int(1);
	delete trydmp;
	int x = *trydmp;*/
	RunEnv::Cnf.BinPath = "C:\\Users\\PC\\source\\repos\\Hmi\\Hmi\\bin\\HmiTest\\HMI\\Hmi.bin";
	//RunEnv::Cnf.BinPath = "..\\..\\Hmi.bin";
	core.Init();
	//atexit(CLOSE_DATASERVER);
	return core.Run();
#endif


}
