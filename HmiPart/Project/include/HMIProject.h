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
	
	//ֱ���ڴ�
	struct BinHeader
	{
		char Magic[16];//XJ-HMI
		unsigned int Size;  //�ļ��ܴ�С
		unsigned int ResOffset;//��Դ�ļ���ƫ��ֵ
		int Crc;//�ļ���У��ֵ
	};
	
	struct HMIProject
	{
		static HMIProject* Ins;
		ProjectInfo Info;//������Ϣ
		ProjectSetting Setting;//������Ϣ
		ProjectDevice  Devices;
		ProjectPortVarsInfo Vars;//��̬�������
		ProjectDataTables Tables;//�����Ϣ
		ProjectPages Pages;//ҳ�����
		ProjectMacros Macros;//�꺯�����		
		ProjectSystemSet SysSetting;
		ProjectResources Reses;//��Դ
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
