/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GifPicControl.h
 * Author   : qiaodan
 * Date     : 2020-12-22
 * Descript : 关于GIF图片控件的控制的头文件，创建GIF图片绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "GifPicModel.h"
#include "GifPicView.h"
#include "HMIWindow.h"
namespace UI
{
	struct GifData
	{
		GifPicModel *Model;
		GifPicView *View;
	};
	class GifPicControl : public BaseControl
	{
	public:
		GifPicControl() :GifPicControl(nullptr) {}
		GifPicControl(HMIPage*);
		void OnReady();//开始
		void CreateView()override;
		GifData GifTimerData;
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);

	protected:
		int PeekHMIMessage(Message::Msg* msg);
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<GifPicModel> mode_;
	};
}
