#pragma once
#include "BaseTest.h"
class Sqlite_Test_Use : BaseTest
{
public:
	static Sqlite_Test_Use& Ins();

	bool Run(int argc, char ** argv);

};