#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TableControl.h
 * Author   : qiaodan
 * Date     : 2020-12-3
 * Descript : ���ڱ��Ŀ��Ƶ�ͷ�ļ�������Բ�����ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "TableControl.h"
#include "DataVar.h"
#include "TableView.h"
#include "TableModel.h"

namespace UI
{
	class TableControl : public BaseControl
	{
	public:
		TableControl() :TableControl(nullptr) {}
		TableControl(HMIPage*);
		void CreateView() override;
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<TableModel> mode_;
	};
}

