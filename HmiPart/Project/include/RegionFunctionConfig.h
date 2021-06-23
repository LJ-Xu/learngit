#pragma once
#include "BtnFunctionRes.h"
#include "MultiPilotLampConfig.h"
#include "MoveableDisplayUnit.h"
namespace Project
{
	enum ActionMode {Startframe, CloseFrame, CoilTrigger, FuncTimer, FuncContinue, FirstDownload, FirstPowerON, NumLogic};
	class RegionFunctionConfig :public MoveableDisplayUnit
	{
	public:
		ActionMode Mode;			//��������ģʽ
		DataVarId CoilVar;			//��Ȧ��������Ȧ��ַ
		int TriggerMode;			//��Ȧ������ʽ	0�������� 1���½��� 
		double Time;				//��ʱʱ��
		DataVarId TimeLmtVar;		//��ʱ/����ģʽ��Ȧ����
		int StopMode;				//��ʱ/����ģʽ��Ȧ����ģʽ 0:OFF,1:ON
		DataVarId TimeValueVarId;	//��ʱ��ʾ��ʱʱ��
		DataVarId ValueVarId;		//��ֵ�߼�������ȡֵ�Ĵ���
		double Value;				//��ֵ�߼��Ա�ֵ
		Conditions NumlogicCond;	//��ֵ�߼�����
		vector<BtnFunctionRes> Functions;				//���ܼ�

		DataVarId FirstDownLoadVar;	//���غ��һ��ɨ���־
		DataVarId PowerOnVar;		//�������һ��ɨ���־

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Mode, CoilVar, TimeValueVarId, ValueVarId, Value, NumlogicCond, PowerOnVar,
				TriggerMode, TriggerMode, Time, TimeLmtVar, StopMode, Functions, FirstDownLoadVar,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}


