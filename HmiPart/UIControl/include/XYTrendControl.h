#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : XYTrendControl.h
 * Author   : qiaodan
 * Date     : 2021-03-12
 * Descript : ����XY����ͼ�ؼ��Ŀ��Ƶ�ͷ�ļ�����������ͼ���ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "XYTrendModel.h"
#include "XYTrendView.h"

namespace UI
{
	class XYTrendControl : public BaseControl
	{
	public:
		XYTrendControl() :XYTrendControl(nullptr) {}
		XYTrendControl(HMIPage*);
		void OnReady();
		void HandleSysChange(SysChangeEM catogray);
		void GetSampleData(void);
		void SetOffSetPos(int offx, int offy);
		bool HasTimer = false;
	protected:
		void CreateView();
		int PeekHMIMessage(Message::Msg* msg);
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<XYTrendModel> mode_;
		bool isSample_ = false;
		bool isStop_ = false;
		bool preSampleStatus_;
		bool preClearStatus_;
	};

}
