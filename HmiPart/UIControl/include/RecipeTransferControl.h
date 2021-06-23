/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeTransferControl.h
 * Author   : qiaodan
 * Date     : 2021-03-02
 * Descript : �����䷽����Ŀ��Ƶ�ͷ�ļ��������䷽������̰������ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "RecipeTransferView.h"
#include "RecipeTransferModel.h"
namespace UI
{
	class RecipeTransferControl : public BaseControl
	{
	public:
		RecipeTransferControl() :RecipeTransferControl(nullptr) {}
		RecipeTransferControl(HMIPage*);
		void CreateView()override;
		void HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();

	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<RecipeTransferModel> mode_;
		vector<bool> perstatus_;
	};
}