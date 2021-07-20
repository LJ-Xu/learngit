/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PinYinPageControl.h
 * Author   : qiaodan
 * Date     : 2021-01-21
 * Descript : 关于拼音界面的控制的头文件，创建字符显示框，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "PinYinPageModel.h"
#include "PinYinPageView.h"
namespace UI
{
	class PinYinPageControl : public BaseControl
	{
	public:
		PinYinPageControl() :PinYinPageControl(nullptr) {}
		PinYinPageControl(HMIPage*);
		void CreateView()override;
		bool HandleSysChange(SysChangeEM catogray);
		void SearchChineseCharacters(const int &currentPage, string pinyin);
		int ChineseNum = 7;
		void SetOffSetPos(int offx, int offy);
		void OnReady();
	protected:

		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<PinYinPageModel> mode_;
	};
}
