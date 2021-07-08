#include "Param.h"
#include <ostream>

static const struct option lopts[] = {
			{ "filepath",  1, 0, 'f' },
			{ "ip",  1, 0, 'i' },
			{ "port",  1, 0, 'p' },
			{ "on-line",  1, 0, 'c' },
			{ "help",  1, 0, 'h' },
			{ nullptr, 0, 0, 0 },
};

void Param::PrintUsage(const char *prog)
{
	printf("Usage: %s [-fh]\n", prog);
	puts(" -f --filepath   	Hmi.bin file path\n"
		 " -i --ip   	    HmiMain Server IP\n"
		 " -p --port         HmiMain Server Port\n"
		 "  -h --help		print the help information\n");
	exit(1);
}

bool Param::ParseParam(int argc, char **argv)
{
	int c;
	while (1) {
		c = getopt_long(argc, argv, "f:h:i:p:c:",lopts, nullptr);
		if (c == -1)
			break;
		switch (c) {
		case 'f':
			RunEnv::Cnf.BinPath = std::string(optarg);
			break;
		case 'p':
			sscanf(optarg, "%d", &RunEnv::Cnf.SrvPort);
			break;
		case 'i':
			RunEnv::Cnf.SrvIP = std::string(optarg);
			break;
		case 'c':
			RunEnv::Cnf.IsOffLine = false;
			break;
		case 'h':
			PrintUsage(argv[0]);
			break;
		default:
			PrintUsage(argv[0]);
			break;
		}
	}
	return true;
}
