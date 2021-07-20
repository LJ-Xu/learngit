/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GDynPicView.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 绘制动态图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "UIComm.h"
#include "ResourceService.h"
#include "GDynPicView.h"
#include "GDynPicControl.h"
#include "GDynPicModel.h"
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include "ImageFormat.h"
#include "RotatePic.h"
#include "Logger.h"
namespace UI
{
	GDynPicView::GDynPicView(int X, int Y, int W, int H) :HMIBaseView(X, Y, W, H) {}
	GDynPicView::~GDynPicView() {}


	void GDynPicView::InitDraw()
	{
		//GetPicRes();		//缓存方式
		SortPicKey();		//非缓存方式
	}
	void GDynPicView::SortPicKey()			//图片keyz值排序（用于非缓存显示）
	{
		shared_ptr<GDynPicModel> model = BaseView.GetModel<GDynPicModel>();
		if (model->PicDynConfig.ImageFmtList.size() == 0)
		{
			LOG_INFO_("DynPic Pic is NULL\n");
			return;
		}
		switch (model->PicDynConfig.SwitchSeq)
		{
		case Project::Seq_Order:
			break;
		case Project::Seq_Reverse:
			std::reverse(model->PicDynConfig.ImageFmtList.begin(), model->PicDynConfig.ImageFmtList.end());
			break;
		case Project::Seq_Random:
		{
			string num;
			for (unsigned i = 0; i < model->PicDynConfig.SwitchOrder.size(); i++)
			{
				if (model->PicDynConfig.SwitchOrder[i] >= '0' &&  model->PicDynConfig.SwitchOrder[i] <= '9')
					num += model->PicDynConfig.SwitchOrder[i];
				else
				{
					SortNum.push_back(atoi(num.c_str()));
					num.erase();
				}
				if (i == model->PicDynConfig.SwitchOrder.size() - 1 && !num.empty())
					SortNum.push_back(atoi(num.c_str()));
			}
			vector<Project::ImageFormat> tmpImagekey;
			for (unsigned i = 0; i < SortNum.size(); i++)
			{
				if ((size_t)SortNum[i] <= model->PicDynConfig.ImageFmtList.size())
					tmpImagekey.push_back(model->PicDynConfig.ImageFmtList[SortNum[i] - 1]);
			}
			vector<Project::ImageFormat>().swap(model->PicDynConfig.ImageFmtList);
			model->PicDynConfig.ImageFmtList = tmpImagekey;
			break;
		}
		default:
			break;
		}
	}
	void GDynPicView::SortPic()
	{
		shared_ptr<GDynPicModel> model = BaseView.GetModel<GDynPicModel>();

		switch (model->PicDynConfig.SwitchSeq)
		{
		case Project::Seq_Order:
			break;
		case Project::Seq_Reverse:
			std::reverse(PicImages.begin(), PicImages.end());
			break;
		case Project::Seq_Random:
		{
			string num;
			for (unsigned i = 0; i < model->PicDynConfig.SwitchOrder.size(); i++)
			{
				if (model->PicDynConfig.SwitchOrder[i] >= '0' &&  model->PicDynConfig.SwitchOrder[i] <= '9')
					num += model->PicDynConfig.SwitchOrder[i];
				else
				{
					SortNum.push_back(atoi(num.c_str()));
					num.erase();
				}
				if(i == model->PicDynConfig.SwitchOrder.size() - 1 && !num.empty())
					SortNum.push_back(atoi(num.c_str()));
			}
			vector<Fl_Image*> tmpImage;
			for (unsigned i = 0; i < SortNum.size(); i++)
			{
				if ((size_t)SortNum[i] <= PicImages.size())
					tmpImage.push_back(PicImages[SortNum[i] - 1]);
			}
			vector<Fl_Image*>().swap(PicImages);
			PicImages = tmpImage;
			break;
		}
		default:
			break;
		}

	}

	void GDynPicView::GetPicRes()
	{
		/*获取并处理图片*/
		shared_ptr<GDynPicModel> model = BaseView.GetModel<GDynPicModel>();
		for (unsigned picNo = 0; picNo < model->PicDynConfig.ImageFmtList.size(); picNo++)
		{
			Fl_Image *RGBAimage = IResourceService::Ins()->GetImage(model->PicDynConfig.ImageFmtList[picNo].KeyVal);
			if (!RGBAimage)
			{
				LOG_INFO("DynPic status%d Pic is NULL\n", model->PicDynConfig.CurrentPic);
				return;
			}
			//设置图片为透明色
			if (RGBAimage->d() != 4)
			{
				uchar *rgb = new uchar[RGBAimage->w() * RGBAimage->h() * RGBAimage->d()];
				uchar *rgba = new uchar[RGBAimage->w() * RGBAimage->h() * 4];
				memcpy(rgb, RGBAimage->data()[0], RGBAimage->w() * RGBAimage->h() * RGBAimage->d());
				int j, i;
				for (i = 0, j = 0; i < RGBAimage->w() * RGBAimage->h() * RGBAimage->d(); i += 3, j += 4)
				{
					rgba[j] = rgb[i];
					rgba[j + 1] = rgb[i + 1];
					rgba[j + 2] = rgb[i + 2];
					if (model->PicDynConfig.ImageFmtList[picNo].UseFaheout)
						rgba[j + 3] = model->PicDynConfig.ImageFmtList[picNo].FadeoutPecent;
					else
						rgba[j + 3] = 255;
				}
				RGBAimage = new Fl_RGB_Image(rgba, RGBAimage->w(), RGBAimage->h(), 4);
			}
			if (model->PicDynConfig.ImageFmtList[picNo].UseRotate)
			{
				Fl_Image *RGBimage = UI::RotatePic::NewRotatePic(RGBAimage, (double)model->PicDynConfig.ImageFmtList[picNo].RotateAngle);
				PicImages.push_back(RGBimage);
			}
			else
				PicImages.push_back(RGBAimage);
				//RotatePic(RGBAimage, (double)model->PicDynConfig.ImageFmtList[picNo].RotateAngle);
		}
		SortPic();
	}
 
	void GDynPicView::draw()
	{
		shared_ptr<GDynPicModel> model = BaseView.GetModel<GDynPicModel>();

		//if (model->PicDynConfig.CurrentPic < PicImages.size())
		//	image(PicImages[model->PicDynConfig.CurrentPic]);
		IResourceService::Ins()->SetRenderStatus(model->PicDynConfig.CurrentPic,
			model->PicDynConfig.ImageFmtList.size());		//获取当前状态
#if 1
		if ((size_t)model->PicDynConfig.CurrentPic < model->PicDynConfig.ImageFmtList.size())
		{
			bool needDelete = false;
			Fl_Image *picRes = IResourceService::Ins()->GetImage(model->PicDynConfig.ImageFmtList[model->PicDynConfig.CurrentPic].KeyVal);
			if (!picRes)
			{
				LOG_INFO("DynPic status%d Pic is NULL\n", model->PicDynConfig.CurrentPic);
				return;
			}
			picRes->draw(model->PicDynConfig.X + model->PicDynConfig.OffX,
				model->PicDynConfig.Y + model->PicDynConfig.OffY);
			//设置图片为透明色
			/*if (picRes->d() != 4)
			{
				uchar *rgb = new uchar[picRes->w() * picRes->h() * picRes->d()];
				uchar *rgba = new uchar[picRes->w() * picRes->h() * 4];
				memcpy(rgb, picRes->data()[0], picRes->w() * picRes->h() * picRes->d());
				int j, i;
				for (i = 0, j = 0; i < picRes->w() * picRes->h() * picRes->d(); i += 3, j += 4)
				{
					rgba[j] = rgb[i];
					rgba[j + 1] = rgb[i + 1];
					rgba[j + 2] = rgb[i + 2];
					if (model->PicDynConfig.ImageFmtList[model->PicDynConfig.CurrentPic].UseFaheout)
						rgba[j + 3] = model->PicDynConfig.ImageFmtList[model->PicDynConfig.CurrentPic].FadeoutPecent;
					else
						rgba[j + 3] = 255;
				}
				delete[] rgb;
				picRes = new Fl_RGB_Image(rgba, picRes->w(), picRes->h(), 4);
				needDelete = true;
			}
			if (model->PicDynConfig.ImageFmtList[model->PicDynConfig.CurrentPic].UseRotate)
			{
				Fl_Image *rotatePicRes = UI::RotatePic::NewRotatePic(picRes, (double)model->PicDynConfig.ImageFmtList[model->PicDynConfig.CurrentPic].RotateAngle);
				resize(model->PicDynConfig.X + model->PicDynConfig.OffX,
					   model->PicDynConfig.Y + model->PicDynConfig.OffY,
					   rotatePicRes->w(),rotatePicRes->h());
				rotatePicRes->draw(model->PicDynConfig.X + model->PicDynConfig.OffX - (rotatePicRes->w() - picRes->w()) * 0.5,
					model->PicDynConfig.Y + model->PicDynConfig.OffY - (rotatePicRes->h() - picRes->h()) * 0.5);
				draw_box();
				UI::RotatePic::DeleteRotatePic(rotatePicRes);
				if(needDelete)
					UI::RotatePic::DeleteRotatePic(picRes);
			}
			else
			{
				resize(model->PicDynConfig.X + model->PicDynConfig.OffX,
					model->PicDynConfig.Y + model->PicDynConfig.OffY,
					picRes->w(), picRes->h());
				picRes->draw(model->PicDynConfig.X + model->PicDynConfig.OffX,
					model->PicDynConfig.Y + model->PicDynConfig.OffY);
				if(needDelete)
					UI::RotatePic::DeleteRotatePic(picRes);
			}*/
		}
#endif
		//绘制图片
		//box(FL_NO_BOX);
		//Fl_Box::draw();
		//draw_box();
		//fl_draw_image((const uchar *)imagel->data()[0], model->m_DynPicconfig.x, model->m_DynPicconfig.y,
		//	model->m_DynPicconfig.width, model->m_DynPicconfig.height, 4, model->m_DynPicconfig.width*4);
	}
	int GDynPicView::handle(int event)
	{
		return 0;
	}
}
