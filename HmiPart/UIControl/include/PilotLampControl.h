/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PilotLampControl.h
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : ����ָʾ�ƵĿ��Ƶ�ͷ�ļ�������ָʾ�ƻ��ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "PilotLampModel.h"
#include "PilotLampView.h"
namespace UI
{
	struct PilotLampData
	{
		PilotLampModel *Model;
		PilotLampView *View;
	};
	class PilotLampControl : public BaseControl
	{
	public:
		PilotLampControl() :PilotLampControl(nullptr) {}
		PilotLampControl(HMIPage*  );
		void CreateView()override;
		void OnReady();//��ʼ
		void HandleSysChange(SysChangeEM catogray);
		PilotLampData LampTimerData;
		void SetOffSetPos(int offx, int offy);
		bool HasTimer = false;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr <PilotLampModel> mode_;
	};
}

