#pragma once
#include "stdafx.h"
#include "ProjectPortVarsInfo.h"
#include "HMIProject.h"
#define SYSPFW_COUNT 512
#define SYSPSW_COUNT 512
#define SYSPSB_COUNT 512
#define SPSB_BYTE_COUNT ((SYSPSB_COUNT+7)/8)
#define SPSW_BYTE_COUNT (SYSPSW_COUNT*2)
#define SPFW_BYTE_COUNT (SYSPFW_COUNT*2)
namespace UI
{
	/*
	 ???��ʱ��������ֱ�ӱ��ش洢��������Ҫ֪ͨ�ĵ�ַ
	 */
	class LocalSysDataCache
	{
	public:
		static LocalSysDataCache& Ins();
		char* GetPtr(short regtype, int& len);
		void Set(short regtype,char* data,int len);
		//void Set(short regtype, int addr, const char* data, int len);
		//count��Ҫ����λ���䣬�����ȡ��λ����
		int  Get(short regtype,int addr, char* data, int len,int count=1);
	private:
		LocalSysDataCache();
		LocalSysDataCache(LocalSysDataCache&) = default;
		unique_ptr<WORD> sysPSW_;
		unique_ptr<WORD> sysPFW_;
		unique_ptr<BYTE> sysPSB_;
	};
}

