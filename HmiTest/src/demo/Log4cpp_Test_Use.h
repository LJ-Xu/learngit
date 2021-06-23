#pragma once
#include "BaseTest.h"
class Log4cpp_Test_Use : BaseTest
{
public:
	static Log4cpp_Test_Use& Ins();

	bool Run(int argc, char ** argv);

};

