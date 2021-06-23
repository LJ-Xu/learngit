/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiStatusKeypadControl.h
 * Author   : qiaodan
 * Date     : 2020-10-23
 * Descript : ���ڶ�״̬�����Ŀ��Ƶ�ͷ�ļ���������״̬�����������ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "MultiStatusKeypadView.h"
#include "PermUtility.h"
#include "MultiStatusKeypadModel.h"
namespace UI
{
	class MultiStatusKeypadControl : public BaseControl
	{
	public:
		MultiStatusKeypadControl() :MultiStatusKeypadControl(nullptr) {}
		MultiStatusKeypadControl(HMIPage*);
		void CreateView()override;
		void SetOffSetPos(int offx, int offy);
		void HandleSysChange(SysChangeEM catogray);
		void OnReady();
	protected:

		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<MultiStatusKeypadModel> mode_;
	};
}

