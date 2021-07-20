/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GArcControl.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : ����Բ���Ŀ��Ƶ�ͷ�ļ�������Բ�����ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "GArcView.h"
#include "BaseControl.h"
#include "GArcModel.h"
#include "DataVar.h"

namespace UI
{
	class GArcControl : public BaseControl
	{
	public:
		GArcControl() :GArcControl(nullptr) {}
		GArcControl(HMIPage*);
		void CreateView() override;
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();

	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<GArcModel> mode_;
	};
}