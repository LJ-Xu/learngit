#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmWinControl.h
 * Author   : qiaodan
 * Date     : 2020-11-10
 * Descript : ���ڱ����������Ƶ�ͷ�ļ����������̰������ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "AlarmWinView.h"
#include "AlarmWinModel.h"
#include "AlarmInfoRes.h"

namespace UI
{
	class AlarmWinControl : public BaseControl
	{
	public:
		AlarmWinControl() :AlarmWinControl(nullptr) {}
		AlarmWinControl(HMIPage*);
		void CreateView() override;
		Project::AlarmContent AlarmInfo;
	protected:
		enum DataVarType {	OFFX, OFFY, PERM, NO };
		void AddVarData(vector<Project::DataVarId*> vars) override;
		int PeekHMIMessage(Message::Msg* msg) override;
		DataVarType GetDataVarType(Project::DataVarId &varId);
		void HandleDataVar(DataVarType, Project::DataVarId &varId);
	private:
		std::shared_ptr<AlarmWinModel> mode_;
	};
}


