/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GFuncControl.h
 * Author   : TangYao
 * Date     : 2021/02/07
 * Descript : 函数画布控件控制类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "GFuncModel.h"
#include "GFuncView.h"
#include <iostream>

using namespace std;

namespace UI
{
	struct DCMapDrawFunc {
		int FuncId;
		int Length;
		char * Params;
	};

	class GFuncControl : public BaseControl
	{
	public:
		GFuncControl() : GFuncControl(nullptr) { }
		GFuncControl(HMIPage *);
		~GFuncControl();

	public:
		void CreateView();
	public:
		int GetBackcolor() { return model_->GFuncUnit.BackColor; }
	protected:
		int PeekHMIMessage(Message::Msg * msg) override;

	private:
		std::shared_ptr<GFuncModel> model_;
	};
}

