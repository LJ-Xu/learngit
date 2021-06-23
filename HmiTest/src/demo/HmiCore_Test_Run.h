#pragma once
#include "BaseTest.h"
class HmiCore_Test_Run : BaseTest
{
public:
	static HmiCore_Test_Run& Ins();

	bool Run(int argc, char ** argv);

};
