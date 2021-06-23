#include "SystemInfo.h"

SystemInfo& SystemInfo::Ins()
{
	static SystemInfo inf;
	return inf;
}