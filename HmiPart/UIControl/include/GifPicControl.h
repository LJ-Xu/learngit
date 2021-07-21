/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GifPicControl.h
 * Author   : qiaodan
 * Date     : 2020-12-22
 * Descript : ����GIFͼƬ�ؼ��Ŀ��Ƶ�ͷ�ļ�������GIFͼƬ���ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "GifPicModel.h"
#include "GifPicView.h"
#include "HMIWindow.h"
namespace UI
{
	struct GifData
	{
		GifPicModel *Model;
		GifPicView *View;
	};
	class GifPicControl : public BaseControl
	{
	public:
		GifPicControl() :GifPicControl(nullptr) {}
		GifPicControl(HMIPage*);
		void OnReady();//��ʼ
		void CreateView()override;
		GifData GifTimerData;
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);

	protected:
		int PeekHMIMessage(Message::Msg* msg);
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<GifPicModel> mode_;
	};
}
