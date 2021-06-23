#pragma once
#include "BaseTest.h"
class Breakpad_Test_Use : BaseTest
{
public:
	static Breakpad_Test_Use& Ins();

	bool Run(int argc, char ** argv);

};

