#pragma once
#include <string>
#include "getopt.h"
#include "RunEnv.h"
class Param
{
public:
	RunConfig Cnf;
	bool ParseParam(int argc, char **argv);
	void PrintUsage(const char *prog);
};



