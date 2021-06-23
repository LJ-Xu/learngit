/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyBtnControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڼ��̰����Ŀ��Ƶ�ͷ�ļ����������̰������ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "KeyBtnView.h"
#include "KeyBtnModel.h"
namespace UI
{
	class KeyBtnControl : public BaseControl
	{
	public:
		KeyBtnControl() :KeyBtnControl(nullptr) {}
		KeyBtnControl(HMIPage*  );
		void CreateView()override;
		void HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<KeyBtnModel> mode_;
	};
}


