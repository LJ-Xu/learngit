#include "HmiCore_Test_Run.h"
#include <iostream>
#include "HMICore.h"
#include "HMIProject.h"
#include "RunEnv.h"
#include <atomic>
#include "client/windows/handler/exception_handler.h"
HmiCore_Test_Run& HmiCore_Test_Run::Ins()
{
	HmiCore_Test_Run ins;
	return ins;
} 
bool HmicoreCb(const wchar_t *dump_path, const wchar_t *id,
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

static Core::HmiCore core;
Project::HMIProject prj;
bool HmiCore_Test_Run::Run(int argc, char ** argv)
{
	google_breakpad::ExceptionHandler eh(L".", NULL, HmicoreCb, NULL,
		google_breakpad::ExceptionHandler::HANDLER_ALL);
	const char* path = "F:\\HMI\\source\\src\\Hmi\\Hmi\\bin\\Debug\\hmi.bin";
	//const char* path = "C:\\Users\\PC\\source\\repos\\Hmi\\Hmi\\bin\\Debug\\Hmi.bin";

	//prj.setting.hmiName = "xj-hmi";
	//prj.ToBinFile(path);
	RunConfig cnf(path);
	core.Init(cnf);
	core.Run();

	return true;
}