/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeTransferControl.h
 * Author   : qiaodan
 * Date     : 2021-03-02
 * Descript : 关于配方传输的控制的头文件，创建配方传输键盘按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "RecipeTransferView.h"
#include "RecipeTransferModel.h"
namespace UI
{
	class RecipeTransferControl : public BaseControl
	{
	public:
		RecipeTransferControl() :RecipeTransferControl(nullptr) {}
		RecipeTransferControl(HMIPage*);
		void CreateView()override;
		void HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();

	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<RecipeTransferModel> mode_;
		vector<bool> perstatus_;
	};
}