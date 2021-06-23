#include "Gtest_test_Use.h"
#include <iostream>
#include "gtest/gtest.h"

Gtest_test_Use& Gtest_test_Use::Ins()
{
	Gtest_test_Use ins;
	return ins;
}

int fun(int a, int b)
{
	return (a - b);
}


TEST(fun, case1)
{
	EXPECT_LT(-2, fun(1, 2));
	EXPECT_EQ(-1, fun(1, 2));
	ASSERT_LT(-2, fun(1, 2));
	ASSERT_EQ(-1, fun(1, 2));
}

bool Gtest_test_Use::Run(int argc, char ** argv)
{
	
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	return true;
}
