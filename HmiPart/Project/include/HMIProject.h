#pragma once
#include "ProjectPortVarsInfo.h"
#include "CerealCommon.hpp"
#include "ProjectInfo.h"
#include "ProjectDataTables.h"
#include "ProjectMacros.h"
#include "ProjectPages.h"
#include "ProjectResources.h"
#include "ProjectSetting.h"
#include "ProjectDevice.h"
#include "ProjectSystemSet.h"
#ifdef WIN32

#include <tlhelp32.h> 
#endif

namespace Project
{
	
	//直接内存
	struct BinHeader
	{
		char Magic[16];//XJ-HMI
		unsigned int Size;  //文件总大小
		unsigned int ResOffset;//资源文件的偏移值
		int Crc;//文件的校验值
	};
	
	struct HMIProject
	{
		static HMIProject* Ins;
		ProjectInfo Info;//工程信息
		ProjectSetting Setting;//设置信息
		ProjectDevice  Devices;
		ProjectPortVarsInfo Vars;//动态变量相关
		ProjectDataTables Tables;//表格信息
		ProjectPages Pages;//页面相关
		ProjectMacros Macros;//宏函数相关		
		ProjectSystemSet SysSetting;
		ProjectResources Reses;//资源
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Info, Setting, Devices, Vars, Tables, Pages,Macros, SysSetting, Reses);
		}
		int ToBinFile(const char* path);
		int LoadBinFile(const char* path);
		/*
		int ToMem();
		int LoadMem();
		*/
	};


}
