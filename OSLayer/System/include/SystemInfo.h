#pragma once
#include <string>
struct HardInfo
{
	std::string Arch; //intel arm 
	short ArchBits;//32λ 64λ
	std::string Ver;

};
struct SoftInfo
{
	std::string Name;
};
class SystemInfo
{
public:
	static SystemInfo& Ins();
	HardInfo HardInf;
	SoftInfo SoftInf;
	
};

