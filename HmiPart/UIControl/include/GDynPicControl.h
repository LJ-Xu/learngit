/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GDynPicControl.h
 * Author   : qiaodan
 * Date     : 2020-12-29
 * Descript : ���ڶ�̬ͼƬ�ؼ��Ŀ��Ƶ�ͷ�ļ���������̬ͼƬ���ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "GDynPicView.h"
#include "GDynPicModel.h"
namespace UI
{
	struct DynPicData
	{
		GDynPicView *View;
		GDynPicModel *Model;
	};
	void DynPicTimerCb(void *data);

	class GDynPicControl : public BaseControl
	{
	public:
		GDynPicControl() :GDynPicControl(nullptr) {}
		GDynPicControl(HMIPage*);
		void CreateView()override;
		void OnReady();//��ʼ
		DynPicData PicTimerData;
		void SetOffSetPos(int offx, int offy);
		bool HandleSysChange(SysChangeEM catogray);
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varIds);
	private:
		std::shared_ptr<GDynPicModel> mode_;
		bool isswitch_ = false;
		bool prestartflag_ = false;
		bool prestopflag_ = false;
	};
}
