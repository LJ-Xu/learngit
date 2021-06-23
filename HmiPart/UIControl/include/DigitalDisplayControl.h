/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalDisplayControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ������ֵ��ʾ��Ŀ��Ƶ�ͷ�ļ���������ֵ��ʾ�򣬴��ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "DigitalDisplayModel.h"
#include "DigitalDisplayView.h"
namespace UI
{
	struct DigitalDisplayData
	{
		DigitalDisplayView *View;
		DigitalDisplayModel *Model;
	};
	class DigitalDisplayControl : public BaseControl
	{
	public:
		DigitalDisplayControl() :DigitalDisplayControl(nullptr) {}
		DigitalDisplayControl(HMIPage*);
		void CreateView()override;
		void HandleSysChange(SysChangeEM catogray);
		void OnReady();		//��ʼ
		void SetOffSetPos(int offx, int offy);

		DigitalDisplayData TimerData;
		bool HasTimer = false;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<DigitalDisplayModel> mode_;
	};
}
