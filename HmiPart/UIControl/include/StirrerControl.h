/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerControl.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 搅拌器控件控制头文件
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "StirrerModel.h"
#include <iostream>
#include <cstdio>

using namespace std;

namespace UI
{
	void StirrerCB(void * data);
	class StirrerControl : public BaseControl
	{
	public:
		StirrerControl() : StirrerControl(nullptr){ }
		StirrerControl(HMIPage *);
		~StirrerControl();

	public:
		void CreateView();
		void OnReady() override;
		void HandleSysChange(SysChangeEM catogray);

	protected:
		int PeekHMIMessage(Message::Msg * msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:


	private:
		shared_ptr<StirrerModel> model_;
	};
}

