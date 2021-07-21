/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeypadControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڰ����Ŀ��Ƶ�ͷ�ļ����������̰������ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "KeypadView.h"
#include "KeypadModel.h"
namespace UI
{
	class KeypadControl : public BaseControl
	{
	public:
		KeypadControl() :KeypadControl(nullptr) {}
		KeypadControl(HMIPage*);
		void CreateView()override;
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<KeypadModel> mode_;
	};
}
