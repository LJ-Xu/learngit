#pragma once
#include <string>
#include "getopt.h"
#include "RunEnv.h"
class Param
{
public:
	bool ParseParam(int argc, char **argv);
	void PrintUsage(const char *prog);
};



