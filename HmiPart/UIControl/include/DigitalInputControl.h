/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalInputControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ������ֵ�����Ŀ��Ƶ�ͷ�ļ���������ֵ����򣬴��ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "DigitalInputView.h"
#include "PermUtility.h"
#include "DigitalInputModel.h"
namespace UI
{
	struct DigitalInputData
	{
		DigitalInputView *View;
		DigitalInputModel *Model;
	};
	class DigitalInputControl : public BaseControl
	{
	public:
		DigitalInputControl() :DigitalInputControl(nullptr) {}
		DigitalInputControl(HMIPage*  );
		void CreateView()override;
		void HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);

		DigitalInputData TimerData;
		void OnReady();		//��ʼ
		bool HasTimer = false;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<DigitalInputModel> mode_;
	};
}

