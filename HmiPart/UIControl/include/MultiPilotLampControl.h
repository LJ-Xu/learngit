/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiPilotLampControl.h
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : ���ڶ�״ָ̬ʾ�ƵĿ��Ƶ�ͷ�ļ���������״ָ̬ʾ�ƻ��ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "MultiPilotLampModel.h"
#include "MultiPilotLampView.h"

namespace UI
{
	//class MultiPilotLampView;
	struct MultiPilotLampData
	{
		MultiPilotLampView *View;
		MultiPilotLampModel *Model;
	};

	class MultiPilotLampControl : public BaseControl
	{
	public:
		MultiPilotLampControl() :MultiPilotLampControl(nullptr) {}
		MultiPilotLampControl(HMIPage*);
		void CreateView() override;
		void HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);

		void OnReady();		//��ʼ
		MultiPilotLampData MultiLampTimerData;
		bool HasTimer = false;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);

		/*�������ݼĴ����仰*/
		void HandleRegChange();
		void HandleMeetCondition(int i);
		/*******************************************************************************
		 * Name     : MeetCondition
		 * Descript : �ж��Ƿ�����״̬����
		 * Input    : cond  - ��ǰ״̬����
		 *			  src   - �Ա�ֵ1
		 *			  dst   - �Ա�ֵ1
		 * Output   : true  - ��������
		 *			  flase - ����������
		 * Note	    :
		 *******************************************************************************/
		//bool MeetCondition(Project::Conditions cond, double src, double dst);
		bool MeetCondition(Project::StatusCondition cond, XJDataType* tp);

	private:
		std::shared_ptr<MultiPilotLampModel> mode_;
		double readvalue_;			//��ȡ��ȡ����
		bool readbit_;
	};
}
