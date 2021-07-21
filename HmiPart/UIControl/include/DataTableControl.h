#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DataTableControl.h
 * Author   : qiaodan
 * Date     : 2021-03-26
 * Descript : 关于数据表格控制的头文件，创建数据表格绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "DataTableView.h"
#include "DataTableModel.h"
namespace UI
{
	class DataTableControl : public BaseControl
	{
	public:
		DataTableControl() :DataTableControl(nullptr) {}
		DataTableControl(HMIPage*);
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int pageoffx, int pageoffy);
		void OnReady();
	protected:
		void CreateView()override;
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<DataTableModel> mode_;
	};
}