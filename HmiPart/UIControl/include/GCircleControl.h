/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GCircleControl.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : ����Բ�Ŀ��Ƶ�ͷ�ļ�������Բ���ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "GCircleView.h"
#include "BaseControl.h"
#include "GCircleModel.h"
namespace UI
{
	class GCircleControl : public BaseControl
	{
	public:
		GCircleControl() :GCircleControl(nullptr) {}
		GCircleControl(HMIPage*  );
		void CreateView()override;
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();
	protected:

		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<GCircleModel> mode_;
	};
}
