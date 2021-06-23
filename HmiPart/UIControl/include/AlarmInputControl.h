#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmInputControl.h
 * Author   : qiaodan
 * Date     : 2020-11-3
 * Descript : ���ڱ���¼��Ŀ��Ƶ�ͷ�ļ����������̰������ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "AlarmInputView.h"
#include "AlarmInputModel.h"
namespace UI
{
	class AlarmInputControl : public BaseControl
	{
	public:
		AlarmInputControl() :AlarmInputControl(nullptr) {}
		AlarmInputControl(HMIPage*);
		void CreateView() override;
		vector<Project::AlarmContent> Content;				//���������б�
		vector<Project::AlarmDisPlayInfo> DisplayInfo;		//������ʾ�ؼ��б�
		vector<Project::AlarmWinInfo> WinInfo;		//�������ڿؼ��б�

		/*******************************************************************************
		 * Name     : MeetByteCondition
		 * Descript : �ж��Ƿ�����λ���ݱ�����������
		 * Input    : cond  - �ж�����
		 *			  src   - ԭ����
		 *			  dst   - �Ա�����
		 * Output   : true  - ���㱨������
		 *			  false - �����㱨������
		 * Note	    :
		 *******************************************************************************/
		bool MeetByteCondition(Project::Conditions cond, int src, int dst);
		bool MeetBitCondition(Project::BitConditions cond, int src, int dst);


		/*******************************************************************************
		 * Name     : MeetAlarmCondtion
		 * Descript : ���Ա�����Ϣ�Ƿ���Ҫ����
		 * Input    : varId - �����仯�ļĴ�����ַ
		 * Output   : 0 - ����Ҫ���� 
		 *			  1 - ��Ҫ����
		 * Note	    :
		 *******************************************************************************/
		int MeetAlarmCondtion(Project::DataVarId &varId);


		/*******************************************************************************
		 * Name     : RecoverAlarm
		 * Descript : �жϲ����±�����Ϣ�Ƿ�ָ�
		 * Input    : info - ��ǰ������Ϣ����
		 * Output   : 0 - ������Ϣδ����
		 *			  1 - ������Ϣ����
		 * Note	    :
		 *******************************************************************************/
		int RecoverAlarm(Project::AlarmInfoRes &info);

		/*******************************************************************************
		 * Name     : StoreAlarmInfo
		 * Descript : �洢������Ϣ
		 * Input    : info - ������������Ϣ
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void StoreAlarmInfo(Project::AlarmInfoRes &info);
	protected:
		enum DataVarType { ALARM, RECOVER, NAME, NO };

		int PeekHMIMessage(Message::Msg* msg) override;
		DataVarType GetDataVarType(Project::DataVarId &varId);
		void HandleDataVar(DataVarType, Project::DataVarId &varId);
	private:
		std::shared_ptr<AlarmInputModel> mode_;
		int prevbitValue_;		//��¼��һ��λ�Ĵ���ֵ
	};
}