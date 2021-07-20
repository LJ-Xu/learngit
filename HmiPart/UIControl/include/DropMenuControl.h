#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DropMenuControl.h
 * Author   : qiaodan
 * Date     : 2020-12-3
 * Descript : 关于表格的控制的头文件，创建圆弧绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "DataVar.h"
#include "DropMenuModel.h"
#include "DropMenuView.h"

namespace UI
{
	class DropMenuControl : public BaseControl
	{
	public:
		DropMenuControl() :DropMenuControl(nullptr) {}
		DropMenuControl(HMIPage*);
		void CreateView() override;
		void SetOffSetPos(int offx, int offy);
		void OnReady();
		bool HandleSysChange(SysChangeEM catogray);
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<DropMenuModel> mode_;
	};
}
