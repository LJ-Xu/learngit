/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : FanControl.h
 * Author   : qiaodan
 * Date     : 2020-03-31
 * Descript : ���ڷ��ȿؼ��Ŀ��Ƶ�ͷ�ļ����������Ȼ��ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "FanView.h"
#include "FanModel.h"
namespace UI
{
	void FanTimerCb(void *data);

	class FanControl : public BaseControl
	{
	public:
		FanControl() :FanControl(nullptr) {}
		FanControl(HMIPage*);
		void CreateView()override;
		void OnReady();//��ʼ
		void SetOffSetPos(int offx, int offy);
		bool HandleSysChange(SysChangeEM catogray);
		void HandleFanTimeOut();
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varIds);
	private:
		std::shared_ptr<FanModel> mode_;
		bool isswitch_ = false;
	};
}
