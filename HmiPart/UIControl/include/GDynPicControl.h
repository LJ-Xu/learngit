/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GDynPicControl.h
 * Author   : qiaodan
 * Date     : 2020-12-29
 * Descript : 关于动态图片控件的控制的头文件，创建动态图片绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "GDynPicView.h"
#include "GDynPicModel.h"
namespace UI
{
	struct DynPicData
	{
		GDynPicView *View;
		GDynPicModel *Model;
	};
	void DynPicTimerCb(void *data);

	class GDynPicControl : public BaseControl
	{
	public:
		GDynPicControl() :GDynPicControl(nullptr) {}
		GDynPicControl(HMIPage*);
		void CreateView()override;
		void OnReady();//开始
		DynPicData PicTimerData;
		void SetOffSetPos(int offx, int offy);
		bool HandleSysChange(SysChangeEM catogray);
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varIds);
	private:
		std::shared_ptr<GDynPicModel> mode_;
		bool isswitch_ = false;
		bool prestartflag_ = false;
		bool prestopflag_ = false;
	};
}
