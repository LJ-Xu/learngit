/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnFunctionControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于功能按键的控制的头文件，创建键盘按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "BtnFunctionView.h"
#include "BtnFunctionModel.h"
namespace UI
{
	class BtnFunctionControl : public BaseControl
	{
	public:
		BtnFunctionControl() :BtnFunctionControl(nullptr) {}
		BtnFunctionControl(HMIPage*);
		void SetOffSetPos(int offx, int offy);
		void CreateView()override;
		void HandleSysChange(SysChangeEM catogray);
		bool HandlePermLimit(int region, bool popwin);
		void OnReady();
		/*******************************************************************************
		 * Name     : HandleBtnFunc
		 * Descript : 根据状态调用对应处理函数
		 * Input    : action - 按键状态
		 *******************************************************************************/
		bool HandleBtnFunc(int action);
		vector<Project::BtnFunctionRes> TimerTask;			//定时器任务
		void HandleSetData(Project::FunctionSetup &param);

		bool StartTimer = false;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<BtnFunctionModel> mode_;
		bool switchwin_ = false;

		void HandleSetCoil(Project::FunctionSetup param);
		void HandleArithmetic(Project::FunctionSetup param);
		void HandleSendData(Project::FunctionSetup param);
		void HandleSwitchScreen(Project::FunctionSetup param);//切换画面
		void HandleOpenWin(Project::FunctionSetup param);		//打开窗口
		void HandleCloseWin(Project::FunctionSetup param);		//关闭窗口
		void HandleImportCSVdata(Project::FunctionSetup param);
		void HandleExportCSVdata(Project::FunctionSetup param);
		void HandleDownloadRecipe(Project::FunctionSetup param);
		void HandleUploadRecipe(Project::FunctionSetup param);
		void HandleCallbackFunc(Project::FunctionSetup param);
		void HandlePrintScreen(Project::FunctionSetup param);

		bool needswitchwin_ = false;
		int switchwinno_ = 0;
	};
}

