/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterInputControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : �����ַ������Ŀ��Ƶ�ͷ�ļ��������ַ�����򣬴��ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "CharacterInputModel.h"
#include "CharacterInputView.h"
#include "PermUtility.h"
namespace UI
{
	class CharacterInputControl : public BaseControl
	{
	public:
		CharacterInputControl() :CharacterInputControl(nullptr) {}
		CharacterInputControl(HMIPage*  );
		void SetOffSetPos(int offx, int offy);
		void CreateView()override;
		void HandleSysChange(SysChangeEM catogray);
		void OnReady();
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);

	private:
		std::shared_ptr<CharacterInputModel> mode_;
	};
}

