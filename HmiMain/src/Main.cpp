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

#include "Main.h"
#include "HmiCore.h"
#include "RunEnv.h"
#include "HMIProject.h"
#include "Param.h"
#include "Logger.h"


static Core::HmiCore core;
//Project::HMIProject prj;
//int GenerateMiniDump(HANDLE hFile, PEXCEPTION_POINTERS pExceptionPointers, const char* pwAppName)
//{
//	BOOL bOwnDumpFile = FALSE;
//	HANDLE hDumpFile = hFile;
//	MINIDUMP_EXCEPTION_INFORMATION ExpParam;
//
//	typedef BOOL(WINAPI * MiniDumpWriteDumpT)(
//		HANDLE,
//		DWORD,
//		HANDLE,
//		MINIDUMP_TYPE,
//		PMINIDUMP_EXCEPTION_INFORMATION,
//		PMINIDUMP_USER_STREAM_INFORMATION,
//		PMINIDUMP_CALLBACK_INFORMATION
//		);
//
//	MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
//	HMODULE hDbgHelp = LoadLibrary("DbgHelp.dll");
//	if (hDbgHelp)
//		pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
//
//	if (pfnMiniDumpWriteDump)
//	{
//		if (hDumpFile == NULL || hDumpFile == INVALID_HANDLE_VALUE)
//		{
//			//TCHAR szPath[MAX_PATH] = { 0 };
//			TCHAR szFileName[MAX_PATH] = { 0 };
//			//TCHAR* szAppName = pwAppName;
//			const char* szVersion = "v1.0";
//			TCHAR dwBufferSize = MAX_PATH;
//			SYSTEMTIME stLocalTime;
//
//			GetLocalTime(&stLocalTime);
//			//GetTempPath(dwBufferSize, szPath);
//
//			//wsprintf(szFileName, L"%s%s", szPath, szAppName);
//			CreateDirectory(szFileName, NULL);
//
//			wsprintf(szFileName, "%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp",
//				//szPath, szAppName, szVersion,
//				szVersion,
//				stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
//				stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
//				GetCurrentProcessId(), GetCurrentThreadId());
//			hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
//				FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
//
//			bOwnDumpFile = TRUE;
//			OutputDebugString(szFileName);
//		}
//
//		if (hDumpFile != INVALID_HANDLE_VALUE)
//		{
//			ExpParam.ThreadId = GetCurrentThreadId();
//			ExpParam.ExceptionPointers = pExceptionPointers;
//			ExpParam.ClientPointers = FALSE;
//
//			pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
//				hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &ExpParam : NULL), NULL, NULL);
//
//			if (bOwnDumpFile)
//				CloseHandle(hDumpFile);
//		}
//	}
//
//	if (hDbgHelp != NULL)
//		FreeLibrary(hDbgHelp);
//
//	return EXCEPTION_EXECUTE_HANDLER;
//}
//
//LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
//{
//	if (IsDebuggerPresent())
//	{
//		return EXCEPTION_CONTINUE_SEARCH;
//	}
//
//	return GenerateMiniDump(NULL, lpExceptionInfo, "test");
//}

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
	TCHAR szCommandLine[] = TEXT("HmiDataServer.exe");
	if (CreateProcess(NULL, szCommandLine, NULL, NULL,
		FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &PI))
	{
		return 1;
	}
	else
#endif
		return 1;
}
void CLOSE_DATASERVER()
{
#ifdef WIN32
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
}
static void LoadParam(Param& param, int argc, char ** argv)
{
	
	RunEnv::Init();
	param.ParseParam(argc, argv);
	/*if (!param.Cnf.BinPath.empty())
		RunEnv::Cnf.BinPath = param.Cnf.BinPath;
	if (!param.Cnf.SrvIP.empty())
		RunEnv::Cnf.SrvIP = param.Cnf.SrvIP;
	if (!param.Cnf.AlarmPath.empty())
		RunEnv::Cnf.AlarmPath = param.Cnf.AlarmPath;
	if (!param.Cnf.SamplePath.empty())
		RunEnv::Cnf.SamplePath = param.Cnf.SamplePath;
	if (!param.Cnf.OperationPath.empty())
		RunEnv::Cnf.OperationPath = param.Cnf.OperationPath;*/
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

	//if (!OPEN_DATASERVER())
	//	return 0;
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
