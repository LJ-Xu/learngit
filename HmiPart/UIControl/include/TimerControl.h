/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TimerControl.h
 * Author   : qiaodan
 * Date     : 2021-04-01
 * Descript : ���ڶ�ʱ���Ŀ��Ƶ�ͷ�ļ������ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "TimerView.h"
#include "TimerModel.h"
namespace UI
{
	class TimerControl : public BaseControl
	{
	public:
		TimerControl() :TimerControl(nullptr) {}
		TimerControl(HMIPage*);
		void CreateView()override;
		void OnReady();//��ʼ
		void OnClose();
		void OnStartPage();		//���濪ʼ
		void OnClosePage();		//�������
		void SetOffSetPos(int offx, int offy);
		bool HandleSysChange(SysChangeEM catogray);

		DDWORD TimeCount = 0;
		void HandleTime();
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varIds);
	private:
		std::shared_ptr<TimerModel> mode_;
		bool hastimer_ = false;
		bool perstartbitstatus_ = false;
		bool perstopbitstatus_ = false;
		//bool perresetstatus_ = false;
		double pervalue_ = 0;
		bool starttimer_ = false;		//�Ƿ�ʼ��ʱ
		bool isresettimer_ = false;
	};
}
