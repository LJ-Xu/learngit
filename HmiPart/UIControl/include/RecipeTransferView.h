/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeTransferView.h
 * Author   : qiaodan
 * Date     : 2021-03-02
 * Descript : �����䷽����Ļ���ͷ�ļ��������䷽����
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
#include "RecipeTransferModel.h"
#include "RecipeTransferControl.h"

using namespace std;

namespace UI
{
	class RecipeTransferView : public HMIBaseButton
	{
	public:
		RecipeTransferView(int X, int Y, int W, int H, const char *L);
		~RecipeTransferView();
		void InitDraw();
		int handle(int event);
		void HandleClick(bool push);
		bool HandleOperatePush();

		BasicView BaseView;
		//bool HasShortOnEvent = false;
		bool IsReleased = true;
		bool HaveOperateLimit = false;
		bool DownloadRecipe = false;			//�����䷽
		bool UploadRecipe = false;				//�ϴ��䷽
		int CurrentStatus = 0;

		vector<Project::ColDataTypeInfo> ColInfo;
	protected:
		void draw() override;
	};
}
