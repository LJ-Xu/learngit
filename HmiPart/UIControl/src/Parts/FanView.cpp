/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : FanView.cpp
 * Author   : qiaodan
 * Date     : 2021-03-31
 * Descript : 绘制风扇
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "UIComm.h"
#include "ResourceService.h"
#include "FanView.h"
#include "FanControl.h"
#include "FanModel.h"
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl.H>

namespace UI
{
	FanView::FanView(int X, int Y, int W, int H) :HMIBaseView(X, Y, W, H) {}
	FanView::~FanView() {}


	void FanView::InitDraw()
	{
		shared_ptr<FanModel> model = BaseView.GetModel<FanModel>();
		if (model->FanUnitConfig.RotateDir)
			std::reverse(model->FanUnitConfig.PicKeys.begin(), model->FanUnitConfig.PicKeys.end());
	}
	
	void FanView::draw()
	{
		shared_ptr<FanModel> model = BaseView.GetModel<FanModel>();
		IResourceService::Ins()->SetRenderStatus(CurrentPic, model->FanUnitConfig.PicKeys.size());		//获取当前状态
		if ((size_t)CurrentPic < model->FanUnitConfig.PicKeys.size())
		{
			Fl_Image *picRes = IResourceService::Ins()->GetImage(model->FanUnitConfig.PicKeys[CurrentPic].KeyVal);
			if (!picRes)
			{
				LOG_INFO("Fan State%d is NULL\n",CurrentPic);
				return;
			}
			picRes->draw(model->FanUnitConfig.X + model->FanUnitConfig.OffX,
					model->FanUnitConfig.Y + model->FanUnitConfig.OffY);
		}
		//绘制图片
		//box(FL_NO_BOX);
		//Fl_Box::draw();
		//draw_box();
		//fl_draw_image((const uchar *)imagel->data()[0], model->m_DynPicconfig.x, model->m_DynPicconfig.y,
		//	model->m_DynPicconfig.width, model->m_DynPicconfig.height, 4, model->m_DynPicconfig.width*4);
	}
}
