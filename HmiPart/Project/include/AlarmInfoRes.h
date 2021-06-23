#pragma once
#include <string>
#include <vector>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "DataVarInfo.h"
#include "MultiPilotLampConfig.h"
#include "SaveFileRes.h"
#include "StatusRes.h"
#include "VarStringConfig.h"
#include <time.h>
//#ifdef WIN32
//#include <windows.h>
//#include <winsock.h>
//#else
//#include <sys/time.h>
//#endif
using namespace std;
//#ifdef WIN32
//
//static int gettimeofday(timeval *tp, void *tzp)
//{
//	time_t clock;
//	struct tm tm;
//	SYSTEMTIME wtm;
//
//	GetLocalTime(&wtm);
//	tm.tm_year = wtm.wYear - 1900;
//	tm.tm_mon = wtm.wMonth - 1;
//	tm.tm_mday = wtm.wDay;
//	tm.tm_hour = wtm.wHour;
//	tm.tm_min = wtm.wMinute;
//	tm.tm_sec = wtm.wSecond;
//	tm.tm_isdst = -1;
//	clock = mktime(&tm);
//	tp->tv_sec = clock;
//	tp->tv_usec = wtm.wMilliseconds * 1000;
//	return (0);
//}
//#endif

namespace Project
{
	struct SearchTime
	{
		time_t tv_sec;
		long long tv_usec;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(tv_sec, tv_usec);
		}
		
	};

	struct AlarmDisPlayInfo
	{
		Project::WinCtrlID CtrlId;			//������ʾ�ؼ�ID
		int GroupLowerLmt;				//��ʾ�����鷶Χ����
		int GroupUpperLmt;				//��ʾ�����鷶Χ����
		bool NeedNotify;				//�Ƿ���Ҫ֪ͨ
	};

	struct AlarmWinInfo
	{
		Project::WinCtrlID CtrlId;			//���������ؼ�ID
		int GroupName;					//������
		int GroupNo;					//�������
		bool NeedNotify;				//�Ƿ���Ҫ֪ͨ

		int InfoMode;					//0:�ָ�	1:����
	}; 
	enum AlarmLevel { LOW, NOEMAL, HIGH, URGENT };	//�͡��������ߡ�����

	struct AlarmContent
	{
		int GroupName;
		int GroupNo;
		AlarmLevel Category;	//��������
		SearchTime TriggerTime;		//��������ʱ��
		string Msg;				//������Ϣ
		int Count;				//��������
		SearchTime ConfirmTime;		//����ȷ��ʱ��
		SearchTime RecoverTime;		//�����ָ�ʱ��
		bool Confirmed = false;			//������ȷ��
		bool Recovered = false;			//�����ѻָ�
	};

	struct AlarmMonitorUnit
	{
		std::string VariableName;
		int MonitorType;//0��ص�ַ��1��̬���ִ�
		DataVarId MonitorAddrRef;//ָ��״̬�ļĴ���
		vector<VarStringStyle> StatusStyle;//{MonitorType=1}ÿ��״̬����ʽ
		int AddrType;//{MonitorType=0}0����1�ַ�2����
		bool IsPreLead;//{MonitorType=0}{AddrType=0}ǰ��
		int Intgerdigits;//{MonitorType=0}{AddrType=0}����λ
		int Decimaldigits;//{MonitorType=0}{AddrType=0}С��λ	
		int EncodeMode;//{MonitorType=0}{AddrType!=0}0:utf-8;1:utf-16;2:ascii,3:gb2312
		void Parse(rapidjson::Value& jsonObj);
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(VariableName, MonitorType, MonitorAddrRef, StatusStyle, AddrType, IsPreLead, Intgerdigits, Intgerdigits, Decimaldigits, EncodeMode);
		}
	};

	struct AlarmInfoContext
	{
		std::vector<std::pair<std::string,int>> AlarmContentList;//û�е�ַ��ӦʱintΪ-2������ΪMonitorUnits��Ӧ�±�
		std::vector<AlarmMonitorUnit> MonitorUnits;
		void Parse(rapidjson::Value& jsonObj);
		void Parse(const char* StrContent);
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(AlarmContentList, MonitorUnits);
		}
	};

	enum BitConditions:int	{ ON, OFF, FALL, RISE	};
	struct AlarmInfoRes
	{
		int GroupNo;						//������α��
		int GroupName;						//������������
		string GroupSecName;				//����������
		AlarmLevel Category;				//��������

		int RegType;						//0:λ�Ĵ���	1���ּĴ���
		DataVarId ReadRegVar;				//��ȡ�Ĵ�����ַ
		Conditions BtyeCondition;			//�ּĴ�����������
		BitConditions BitCondition;			//λ�Ĵ�����������

		float ComPareValue;					//�Ա�ֵ
		DataVarId ComPareVar;				//�ԱȼĴ�����ַ
		StatusRes Text;						//������Ϣ
		int UseBeep;						//�Ƿ�ʹ�÷����� 0:�� 1:��
		int BeepTimeOut;					//��������ʱʱ��
		int Count = 0;						//������¼��������

		bool UseAlarmPopWin;				//�Ƿ�ʹ�ñ�������	
		int PopWinNo;							//�����������
		int PopMode;						//����ģʽ 0:����һ�� 1:���ڵ���
		int PopTime;						//��������
		bool IsCloseWin;					//����������رմ���
		bool ChangeFlag = false;
		char TempByteState=-1;//��ʱ����λ״̬
		AlarmInfoContext Context;//�������
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(GroupNo, GroupName, Category, RegType, ReadRegVar,
				BtyeCondition, BitCondition, ComPareValue, ComPareVar, Text, UseBeep, BeepTimeOut,
				UseAlarmPopWin, PopWinNo, PopMode, PopTime, IsCloseWin, Context);
		}

		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<AlarmInfoRes>& vector, rapidjson::Value& jsonObj);
	};
}


