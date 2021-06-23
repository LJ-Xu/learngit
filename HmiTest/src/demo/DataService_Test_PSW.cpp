#if 0
#include "DataService_Test_PSW.h"
#include <iostream>
#include <atomic>
DataService_Test_PSW& DataService_Test_PSW::Ins()
{
	DataService_Test_PSW pswTest;
	return pswTest;
}
static DataService::DataVarHelper  varHelper;
static Project::ProjectPortVarsInfo* vars;
static bool Notify(int ctrlId,int val)
{
	static int ct;
	//std::cout << "\nnotify count="<<ct<< std::endl;
	return true;
}
static Project::ProjectPortVarsInfo* InitPortVarsInfo()
{
	int idx = -1;
	Project::DataVarInfo * info;
	vars = new Project::ProjectPortVarsInfo();
	/******************初始化全局DataVarInfo********************/
	vars->StaticVarInfos.push_back(Project::DataVarInfo());
	info = &vars->StaticVarInfos[++idx];
	info->Addr = 0;
	info->Count = 4;
	info->DataSwap = 0;
	info->DevId = 0;
	info->DataType = Project::VarDataType::DT_DWORD;
	info->Next = Project::DataVarId(1);
	info->Prev = Project::DataVarId::NullId;
	info->RegType = Project::SysRegType::RegType_PSW;
	/*info->StaNo = 10;*/
	info->RawAddr = -1;

	vars->StaticVarInfos.push_back(Project::DataVarInfo());
	info = &vars->StaticVarInfos[++idx];
	info->Addr = 2;
	info->Count = 6;
	info->DataSwap = 0;
	info->DevId = 0;
	info->DataType = Project::VarDataType::DT_DWORD;
	info->Next = Project::DataVarId::NullId;
	info->Prev = Project::DataVarId(0);
	info->RegType = Project::SysRegType::RegType_PSW;
	/*info->StaNo = 10;*/
	info->RawAddr = -1;
	/******************初始化dataVars********************/
	vars->DataVars.insert(std::pair<int, Project::WindowVar>(1, Project::WindowVar()));
	Project::WindowVar* winVar = &vars->DataVars[1];
	winVar->DirectVarIds.push_back(Project::DataVarId(0));

	return vars;
}
static void InitPcCompileEnv()
{
	varHelper.SetPortVars(InitPortVarsInfo());
	/*varHelper.SetUINotify(Notify);*/
}
static void FuncRunTime(bool isInit = false)
{
	static clock_t startTime, endTime;
	if(isInit)
	startTime = clock();//计时开始
	else
	{
		endTime = clock();//计时结束
		cout << "The run time is: " << (double)(endTime - startTime) << "ms" << endl;
	}
	

}
//static int count1;
static std::atomic_llong count1;
inline void add(int j)
{
	if (j % 2)
		++count1;
}

std::mutex mlock;


#define DATE_YEAR_IDX 1
#define DATE_YEAR_LEN 2
#define MAKE_PARAM(k) k##_IDX , k##_LEN

int  add(int a, int b)
{
	return a + b;
}

bool DataService_Test_PSW::Run(int argc, char ** argv)
{
	InitPcCompileEnv();
	varHelper.Ins().Init();
	Project::DataVarId id(0);
	FuncRunTime(true);
	for (size_t i = 0; i < 10000; i++)
	{
		mlock.lock();
		varHelper.SetDWord(id, i);
		mlock.unlock();
		/*for (size_t j = 0; j < 50; j++)
		{
			add(j);
		}*/
		 
	}
	FuncRunTime();
	return true;
}

#endif