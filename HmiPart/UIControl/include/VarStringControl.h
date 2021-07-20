/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : VarStringControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڶ�̬���ֿؼ��Ŀ��Ƶ�ͷ�ļ���������̬���ֻ��ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "VarStringModel.h"
namespace UI
{
	class VarStringControl : public BaseControl
	{
	public:
		VarStringControl() :VarStringControl(nullptr) {}
		VarStringControl(HMIPage*  );
		void SetOffSetPos(int offx, int offy);
		bool HandleSysChange(SysChangeEM catogray);
		void CreateView()override;
		void OnReady();
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<VarStringModel> mode_;
	};
}



