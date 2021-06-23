#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeChartControl.h
 * Author   : qiaodan
 * Date     : 2021-02-04
 * Descript : 关于配方表控制的头文件，创建配方表绘制，传递处理消息
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
