#pragma once
#include "BaseTest.h"
class Gtest_test_Use : BaseTest
{
public:
	static Gtest_test_Use& Ins();

	bool Run(int argc, char ** argv);

};

