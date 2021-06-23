#include "Breakpad_Test_Use.h"
#include <cstdio>
#ifdef WIN32
#include "client/windows/handler/exception_handler.h"
#else
#include "client/linux/handler/minidump_descriptor.h"
#include "client/linux/handler/exception_handler.h"
#endif
Breakpad_Test_Use& Breakpad_Test_Use::Ins()
{
	Breakpad_Test_Use ins;
	return ins;
}
bool callback(const wchar_t *dump_path, const wchar_t *id,
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

int mydiv(int x, int y)
{
	int z;
	z = x / y;
	return z;
}

bool Breakpad_Test_Use::Run(int argc, char ** argv)
{
	google_breakpad::ExceptionHandler eh( L".", NULL, callback, NULL,
					google_breakpad::ExceptionHandler::HANDLER_ALL);
	printf("9/3=%d\n", mydiv(9, 3));
	printf("9/0=%d\n", mydiv(9, 0));  //程序将在此崩溃
	printf("8/2=%d\n", mydiv(8, 2));
	system("pause");
	return true;
}
