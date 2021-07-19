#include "version.h"
#include <string>
std::string GetVersionInfo()
{
	char buf[50];
	sprintf(buf, "V%s.%02d%02d%02d",VER_STR,OS_YEAR % 100, OS_MONTH, OS_DAY);
	return buf;
}