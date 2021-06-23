#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeChartControl.h
 * Author   : qiaodan
 * Date     : 2021-02-04
 * Descript : �����䷽����Ƶ�ͷ�ļ��������䷽����ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "RecipeChartView.h"
#include "RecipeChartModel.h"
namespace UI
{
	class RecipeChartControl : public BaseControl
	{
	public:
		RecipeChartControl() :RecipeChartControl(nullptr) {}
		RecipeChartControl(HMIPage*);
		void OnReady();
		void HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void GetSearchRegData();
	protected:
		void CreateView()override;
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<RecipeChartModel> mode_;
	};
}
