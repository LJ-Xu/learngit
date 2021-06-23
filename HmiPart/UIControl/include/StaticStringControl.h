/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StaticStringControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于静态文字控件的控制的头文件，创建静态文字绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "StaticStringModel.h"
#include "StaticStringView.h"
namespace UI
{
	class StaticStringControl : public BaseControl
	{
	public:
		StaticStringControl() :StaticStringControl(nullptr) {}
		StaticStringControl(HMIPage*  );
		void CreateView()override;
		void HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();

	protected:
		int PeekHMIMessage(Message::Msg* msg);
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<StaticStringModel> mode_;
	};

}