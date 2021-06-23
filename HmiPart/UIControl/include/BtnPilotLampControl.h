/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnPilotLampControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ����ָʾ�ư��������Ŀ��Ƶ�ͷ�ļ�������ָʾ�ư������ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include "BtnPilotLampModel.h"
#include "BtnPilotLampView.h"
#include "PermUtility.h"
#include "BaseControl.h"
namespace UI
{
	struct BtnPilotLampData
	{
		BtnPilotLampModel *Model;
		BtnPilotLampView *View;
	};
	class BtnPilotLampControl : public BaseControl
	{
	public:
		BtnPilotLampControl() :BtnPilotLampControl(nullptr) {}
		BtnPilotLampControl(HMIPage*  );
		void CreateView()override;
		void OnReady();//��ʼ
		void HandleSysChange(SysChangeEM catogray);
		//bool HandleOperatePush();
		BtnPilotLampData BtnLampData;
		void SetOffSetPos(int offx, int offy);
		bool HasTimer = false;
	protected:

		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<BtnPilotLampModel> mode_;

	};
}
