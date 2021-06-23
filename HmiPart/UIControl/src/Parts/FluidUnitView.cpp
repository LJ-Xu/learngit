/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : FluidUnitView.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 流体绘制
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "FluidUnitView.h"
#include "FluidUnitControl.h"
#include "FluidUnitModel.h"
#include <FL/Fl.H>
#include "FL/fl_draw.H"
#include "UIDataService.h"
#include "IResourceService.h"
namespace UI
{
	FluidUnitView::FluidUnitView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H) {}
	FluidUnitView::~FluidUnitView() {}

	void FluidUnitView::InitDraw()
	{
		shared_ptr<FluidUnitModel> model = BaseView.GetModel<FluidUnitModel>();
		//获取坐标个数
		pointnum_ = model->FluidConfig.Coordinates.size();

		//画外框
		//获取管道外框数据
		borderwidth_ = model->FluidConfig.PipeParam.FrameWidth;		//管道宽度
		bordercolor_ = fl_rgb_color(RGBColor(model->FluidConfig.PipeParam.FrameColor));
		pipewidth_ = model->FluidConfig.PipeParam.PipeWidth;

		//获取管道数据
		pipecolor_ = fl_rgb_color(RGBColor(model->FluidConfig.PipeParam.PipeColor));
		sliderwidth_ = model->FluidConfig.SliderParam.SliderWidth;

		//画滑块间距
		sliderinterval_ = model->FluidConfig.SliderParam.SliderIntervalWidget;
		sliderlength_ = model->FluidConfig.SliderParam.SliderLength;
		arrowangle_ = model->FluidConfig.SliderParam.ArrowAngle;
		Slidercolor = fl_rgb_color(RGBColor(model->FluidConfig.SliderParam.SliderColorON));

		//初始化坐标数据
		if (model->FluidConfig.FlowDirection == 1)
			std::reverse(model->FluidConfig.Coordinates.begin(), model->FluidConfig.Coordinates.end());
		Coordinates = model->FluidConfig.Coordinates;
	}

	void FluidUnitView::draw()
	{
		shared_ptr<FluidUnitControl> ctrl = BaseView.GetControl<FluidUnitControl>();
		shared_ptr<FluidUnitModel> model = BaseView.GetModel<FluidUnitModel>();
		if (IResourceService::Ins()->IsRenderMode())
		{
			int status;
			IResourceService::Ins()->SetRenderStatus(status, 2);
			if (status == 1)					//设置ON状态
				Slidercolor = fl_rgb_color(RGBColor(model->FluidConfig.SliderParam.SliderColorON));
			else if (status == 0)				//设置OFF状态
				Slidercolor = fl_rgb_color(RGBColor(model->FluidConfig.SliderParam.SliderColorOFF));
		}
		if (sliderlength_ == 0 && sliderinterval_ == 0)
			return;
		if (pointnum_ <= 1)
			return;
		fl_color(active() ? bordercolor_ : fl_inactive(bordercolor_));
		fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, borderwidth_ + pipewidth_);

		fl_begin_line();
		for (unsigned i = 0; i < (size_t)pointnum_; i++)
			fl_vertex(Coordinates[i].X, Coordinates[i].Y);
		fl_end_line();

		
		fl_color(active() ? pipecolor_ : fl_inactive(pipecolor_));
		fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, pipewidth_);
		fl_begin_line();
		for (unsigned i = 0; i < (size_t)pointnum_; i++)
			fl_vertex(Coordinates[i].X, Coordinates[i].Y);
		fl_end_line();
		//获取滑块数据
		//bool flowcondition=UIDataService::Ins().GetBit(model->FluidConfig.FlowVarId);
		//if (flowcondition)		//ON状态
		//	Slidercolor = fl_rgb_color(RGBColor(model->FluidConfig.SliderParam.SliderColorON));
		//else
		//	Slidercolor = fl_rgb_color(RGBColor(model->FluidConfig.SliderParam.SliderColorOFF));
		fl_color(active() ? Slidercolor : fl_inactive(Slidercolor));

		fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderwidth_);
		fl_begin_line();
		for (unsigned i = 0; i < (size_t)pointnum_; i++)
			fl_vertex(Coordinates[i].X, Coordinates[i].Y);
		fl_end_line();

		for (unsigned i = 0; i < (size_t)pointnum_ - 1; i++)
		{

			if (Coordinates[i].X > Coordinates[i + 1].X)	//向左横
			{
				fl_color(active() ? bordercolor_ : fl_inactive(bordercolor_));
				fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, borderwidth_ + pipewidth_);
				fl_line(Coordinates[i].X - (borderwidth_ + pipewidth_) / 2, Coordinates[i].Y, Coordinates[i + 1].X + (borderwidth_ + pipewidth_) / 2, Coordinates[i + 1].Y);
				fl_color(active() ? pipecolor_ : fl_inactive(pipecolor_));
				fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, pipewidth_);
				fl_line(Coordinates[i].X - (pipewidth_) / 2, Coordinates[i].Y, Coordinates[i + 1].X + (pipewidth_) / 2, Coordinates[i + 1].Y);
				fl_color(active() ? Slidercolor : fl_inactive(Slidercolor));
				fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderwidth_);
				fl_line(Coordinates[i].X - sliderwidth_ / 2, Coordinates[i].Y, Coordinates[i + 1].X + sliderwidth_ / 2, Coordinates[i + 1].Y);
				fl_color(active() ? pipecolor_ : fl_inactive(pipecolor_));
				if (model->FluidConfig.SliderParam.SliderType) {
					
					if (i == 0)
					{
						if (i == pointnum_ - 2)  //起点终点
						{
							GraphicDrawHandle::PushClip(Coordinates[i + 1].X, Coordinates[i].Y - pipewidth_ / 2,
								Coordinates[i].X - (Coordinates[i + 1].X), pipewidth_);
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].X + sliderinterval_ / 2 - SliderOffx;
									l >= Coordinates[i + 1].X - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
								{
									fl_begin_line();
									fl_vertex(l, Coordinates[i].Y + pipewidth_ / 2);
									fl_vertex(l - arrowangle_, Coordinates[i].Y);
									fl_vertex(l, Coordinates[i].Y - pipewidth_ / 2);
									fl_end_line();
								}
							}
							fl_pop_clip();
						}
						else //起点
						{
							GraphicDrawHandle::PushClip(Coordinates[i + 1].X + sliderwidth_ / 2, Coordinates[i].Y - pipewidth_ / 2,
								Coordinates[i].X - (Coordinates[i + 1].X + sliderwidth_ / 2), pipewidth_);
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].X + sliderinterval_ / 2 - SliderOffx;
									l >= Coordinates[i + 1].X + sliderwidth_ / 2 - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
								{
									fl_begin_line();
									fl_vertex(l, Coordinates[i].Y + pipewidth_ / 2);
									fl_vertex(l - arrowangle_, Coordinates[i].Y);
									fl_vertex(l, Coordinates[i].Y - pipewidth_ / 2);
									fl_end_line();
								}
							}
							fl_pop_clip();
						}

					}
					else if (i == pointnum_ - 2)
					{
						GraphicDrawHandle::PushClip(Coordinates[i + 1].X, Coordinates[i].Y - pipewidth_ / 2,
							Coordinates[i].X - sliderwidth_ / 2 - (Coordinates[i + 1].X), pipewidth_);
						{
							fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
							for (int l = Coordinates[i].X - sliderwidth_ / 2 + sliderinterval_ / 2 - SliderOffx;
								l >= Coordinates[i + 1].X - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
							{
								fl_begin_line();
								fl_vertex(l, Coordinates[i].Y + pipewidth_ / 2);
								fl_vertex(l - arrowangle_, Coordinates[i].Y);
								fl_vertex(l, Coordinates[i].Y - pipewidth_ / 2);
								fl_end_line();
							}
						}
						fl_pop_clip();
					}
					else
					{
						GraphicDrawHandle::PushClip(Coordinates[i + 1].X + sliderwidth_ / 2, Coordinates[i].Y - pipewidth_ / 2,
							Coordinates[i].X - sliderwidth_ - Coordinates[i + 1].X, pipewidth_);
						{
							fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
							for (int l = Coordinates[i].X - sliderwidth_ / 2 + sliderinterval_ / 2 - SliderOffx;
								l >= Coordinates[i + 1].X + sliderwidth_ / 2 - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
							{
								fl_begin_line();
								fl_vertex(l, Coordinates[i].Y + pipewidth_ / 2);
								fl_vertex(l - arrowangle_, Coordinates[i].Y);
								fl_vertex(l, Coordinates[i].Y - pipewidth_ / 2);
								fl_end_line();
							}

						}
						fl_pop_clip();
					}
				}
				else {
					if (i == 0)
					{
						if (i == pointnum_ - 2)  //起点终点
						{
							GraphicDrawHandle::PushClip(Coordinates[i + 1].X, Coordinates[i].Y - pipewidth_ / 2,
								Coordinates[i].X - (Coordinates[i + 1].X), pipewidth_);
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].X + sliderinterval_ / 2 - SliderOffx;
									l >= Coordinates[i + 1].X - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
									fl_line(l, Coordinates[i].Y + pipewidth_ / 2, l, Coordinates[i].Y - pipewidth_ / 2);
							}
							fl_pop_clip();
						}
						else //起点
						{
							GraphicDrawHandle::PushClip(Coordinates[i + 1].X + sliderwidth_ / 2, Coordinates[i].Y - pipewidth_ / 2,
								Coordinates[i].X - (Coordinates[i + 1].X + sliderwidth_ / 2), pipewidth_);
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].X + sliderinterval_ / 2 - SliderOffx;
									l >= Coordinates[i + 1].X + sliderwidth_ / 2 - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
									fl_line(l, Coordinates[i].Y + pipewidth_ / 2, l, Coordinates[i].Y - pipewidth_ / 2);
							}
							fl_pop_clip();
						}

					}
					else if(i == pointnum_ - 2)
					{
						GraphicDrawHandle::PushClip(Coordinates[i + 1].X, Coordinates[i].Y - pipewidth_ / 2,
							Coordinates[i].X - sliderwidth_ / 2 - (Coordinates[i + 1].X), pipewidth_);
						{
							fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
							for (int l = Coordinates[i].X - sliderwidth_ / 2 + sliderinterval_ / 2 - SliderOffx;
								l >= Coordinates[i + 1].X - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
								fl_line(l, Coordinates[i].Y + pipewidth_ / 2, l, Coordinates[i].Y - pipewidth_ / 2);
						}
						fl_pop_clip();
					}
					else
					{
						GraphicDrawHandle::PushClip(Coordinates[i + 1].X + sliderwidth_ / 2, Coordinates[i].Y - pipewidth_ / 2,
							Coordinates[i].X - sliderwidth_ - Coordinates[i + 1].X, pipewidth_);
						{
							fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
							for (int l = Coordinates[i].X - sliderwidth_ / 2 + sliderinterval_ / 2 - SliderOffx;
								l >= Coordinates[i + 1].X + sliderwidth_ / 2 - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
								fl_line(l, Coordinates[i].Y + pipewidth_ / 2, l, Coordinates[i].Y - pipewidth_ / 2);
						}
						fl_pop_clip();
					}
				}
			}
			else if (Coordinates[i].X < Coordinates[i + 1].X)	//向右横
			{
				fl_color(active() ? bordercolor_ : fl_inactive(bordercolor_));
				fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, borderwidth_ + pipewidth_);
				fl_line(Coordinates[i].X + (borderwidth_ + pipewidth_) / 2, Coordinates[i].Y, Coordinates[i + 1].X - (borderwidth_ + pipewidth_) / 2, Coordinates[i + 1].Y);
				fl_color(active() ? pipecolor_ : fl_inactive(pipecolor_));
				fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, pipewidth_);
				fl_line(Coordinates[i].X + (pipewidth_) / 2, Coordinates[i].Y, Coordinates[i + 1].X - (pipewidth_) / 2, Coordinates[i + 1].Y);
				fl_color(active() ? Slidercolor : fl_inactive(Slidercolor));
				fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderwidth_);
				fl_line(Coordinates[i].X + sliderwidth_ / 2, Coordinates[i].Y, Coordinates[i + 1].X - sliderwidth_ / 2, Coordinates[i + 1].Y);
				fl_color(active() ? pipecolor_ : fl_inactive(pipecolor_));
				if (model->FluidConfig.SliderParam.SliderType) {

					if (i == 0)
					{
						if (i == pointnum_ - 2)
						{
							GraphicDrawHandle::PushClip(Coordinates[i].X, Coordinates[i].Y - pipewidth_ / 2,
								(Coordinates[i + 1].X - (Coordinates[i].X)), pipewidth_);
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].X - sliderinterval_ / 2 + SliderOffx;
									l < Coordinates[i + 1].X + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
								{
									fl_begin_line();
									fl_vertex(l, Coordinates[i].Y + pipewidth_ / 2);
									fl_vertex(l + arrowangle_, Coordinates[i].Y);
									fl_vertex(l, Coordinates[i].Y - pipewidth_ / 2);
									fl_end_line();
								}
							}
							fl_pop_clip();
						}
						else
						{
							GraphicDrawHandle::PushClip(Coordinates[i].X, Coordinates[i].Y - pipewidth_ / 2,
								(Coordinates[i + 1].X - sliderwidth_ / 2 - (Coordinates[i].X)), pipewidth_);
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].X - sliderinterval_ / 2 + SliderOffx;
									l < Coordinates[i + 1].X - sliderwidth_ / 2 + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
								{
									fl_begin_line();
									fl_vertex(l, Coordinates[i].Y + pipewidth_ / 2);
									fl_vertex(l + arrowangle_, Coordinates[i].Y);
									fl_vertex(l, Coordinates[i].Y - pipewidth_ / 2);
									fl_end_line();
								}
							}
							fl_pop_clip();
						}
					}
					else if (i == pointnum_ - 2)
					{
						GraphicDrawHandle::PushClip(Coordinates[i].X + sliderwidth_ / 2, Coordinates[i].Y - pipewidth_ / 2,
							(Coordinates[i + 1].X - (Coordinates[i].X + sliderwidth_ / 2)), pipewidth_);
						{
							fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
							for (int l = Coordinates[i].X + sliderwidth_ / 2 - sliderinterval_ / 2 + SliderOffx;
								l < Coordinates[i + 1].X + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
							{
								fl_begin_line();
								fl_vertex(l, Coordinates[i].Y + pipewidth_ / 2);
								fl_vertex(l + arrowangle_, Coordinates[i].Y);
								fl_vertex(l, Coordinates[i].Y - pipewidth_ / 2);
								fl_end_line();
							}
						}
						fl_pop_clip();
					}
					else
					{
						GraphicDrawHandle::PushClip(Coordinates[i].X + sliderwidth_ / 2, Coordinates[i].Y - pipewidth_ / 2,
							(Coordinates[i + 1].X - sliderwidth_ / 2 - (Coordinates[i].X + sliderwidth_ / 2)), pipewidth_);
						{
							fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
							for (int l = Coordinates[i].X + sliderwidth_ / 2 - sliderinterval_ / 2 + SliderOffx;
								l < Coordinates[i + 1].X - sliderwidth_ / 2 + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
							{
								fl_begin_line();
								fl_vertex(l, Coordinates[i].Y + pipewidth_ / 2);
								fl_vertex(l + arrowangle_, Coordinates[i].Y);
								fl_vertex(l, Coordinates[i].Y - pipewidth_ / 2);
								fl_end_line();
							}
						}
						fl_pop_clip();
					}
				}
				else
				{
					if (i == 0)
					{
						if (i == pointnum_ - 2)
						{
							GraphicDrawHandle::PushClip(Coordinates[i].X, Coordinates[i].Y - pipewidth_ / 2,
								(Coordinates[i + 1].X  - (Coordinates[i].X)), pipewidth_);
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].X - sliderinterval_ / 2 + SliderOffx;
									l < Coordinates[i + 1].X + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
									fl_line(l, Coordinates[i].Y + pipewidth_ / 2, l, Coordinates[i].Y - pipewidth_ / 2);
							}
							fl_pop_clip();
						}
						else
						{
							GraphicDrawHandle::PushClip(Coordinates[i].X, Coordinates[i].Y - pipewidth_ / 2,
								(Coordinates[i + 1].X - sliderwidth_ / 2 - (Coordinates[i].X)), pipewidth_);
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].X - sliderinterval_ / 2 + SliderOffx;
									l < Coordinates[i + 1].X - sliderwidth_ / 2 + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
									fl_line(l, Coordinates[i].Y + pipewidth_ / 2, l, Coordinates[i].Y - pipewidth_ / 2);
							}
							fl_pop_clip();
						}
					}
				
					else if (i == pointnum_ - 2)
					{
						GraphicDrawHandle::PushClip(Coordinates[i].X + sliderwidth_ / 2, Coordinates[i].Y - pipewidth_ / 2,
							(Coordinates[i + 1].X  - (Coordinates[i].X + sliderwidth_ / 2)), pipewidth_);
						{
							fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
							for (int l = Coordinates[i].X + sliderwidth_ / 2 - sliderinterval_ / 2 + SliderOffx;
								l < Coordinates[i + 1].X + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
								fl_line(l, Coordinates[i].Y + pipewidth_ / 2, l, Coordinates[i].Y - pipewidth_ / 2);
						}
						fl_pop_clip();
					}
					else
					{
						GraphicDrawHandle::PushClip(Coordinates[i].X + sliderwidth_ / 2, Coordinates[i].Y - pipewidth_ / 2,
							(Coordinates[i + 1].X - sliderwidth_ / 2 - (Coordinates[i].X + sliderwidth_ / 2)), pipewidth_);
						{
							fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
							for (int l = Coordinates[i].X + sliderwidth_ / 2 - sliderinterval_ / 2 + SliderOffx;
								l < Coordinates[i + 1].X - sliderwidth_ / 2 + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
								fl_line(l, Coordinates[i].Y + pipewidth_ / 2, l, Coordinates[i].Y - pipewidth_ / 2);

						}
						fl_pop_clip();
					}
				}
			}
			else {
				if (Coordinates[i].Y < Coordinates[i + 1].Y)	//向下竖
				{
					fl_color(active() ? bordercolor_ : fl_inactive(bordercolor_));
					fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, borderwidth_ + pipewidth_);
					fl_line(Coordinates[i].X, Coordinates[i].Y + (borderwidth_ + pipewidth_) / 2, Coordinates[i + 1].X, Coordinates[i + 1].Y - (borderwidth_ + pipewidth_) / 2);
					fl_color(active() ? pipecolor_ : fl_inactive(pipecolor_));
					fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, pipewidth_);
					fl_line(Coordinates[i].X, Coordinates[i].Y + (pipewidth_) / 2, Coordinates[i + 1].X, Coordinates[i + 1].Y - (pipewidth_) / 2);
					fl_color(active() ? Slidercolor : fl_inactive(Slidercolor));
					fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderwidth_);
					fl_line(Coordinates[i].X, Coordinates[i].Y + (sliderwidth_) / 2, Coordinates[i + 1].X, Coordinates[i + 1].Y - (sliderwidth_) / 2);
					fl_color(active() ? pipecolor_ : fl_inactive(pipecolor_));
					if (model->FluidConfig.SliderParam.SliderType) {
						if (i == 0)
						{
							if (i == pointnum_ - 2)
							{
								GraphicDrawHandle::PushClip(Coordinates[i].X - pipewidth_ / 2, Coordinates[i].Y,
									pipewidth_, Coordinates[i + 1].Y - (Coordinates[i].Y));
								{
									fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
									for (int l = Coordinates[i].Y - sliderinterval_ / 2 + SliderOffy;
										l <= Coordinates[i + 1].Y + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
									{
										fl_begin_line();
										fl_vertex(Coordinates[i].X + pipewidth_ / 2, l);
										fl_vertex(Coordinates[i].X, l + arrowangle_);
										fl_vertex(Coordinates[i].X - pipewidth_ / 2, l);
										fl_end_line();
									}
								}
								fl_pop_clip();
							}
							else
							{
								GraphicDrawHandle::PushClip(Coordinates[i].X - pipewidth_ / 2, Coordinates[i].Y,
									pipewidth_, Coordinates[i + 1].Y - sliderwidth_ / 2 - (Coordinates[i].Y));
								{
									fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
									for (int l = Coordinates[i].Y - sliderinterval_ / 2 + SliderOffy;
										l <= Coordinates[i + 1].Y - sliderwidth_ / 2 + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
										//fl_line(p[i].x, l, p[i].x, l + sliderinterval);
									{
										fl_begin_line();
										fl_vertex(Coordinates[i].X + pipewidth_ / 2, l);
										fl_vertex(Coordinates[i].X, l + arrowangle_);
										fl_vertex(Coordinates[i].X - pipewidth_ / 2, l);
										fl_end_line();
									}
								}
								fl_pop_clip();
							}
						}
						else if (i == pointnum_ - 2)
						{
							GraphicDrawHandle::PushClip(Coordinates[i].X - pipewidth_ / 2, Coordinates[i].Y + sliderwidth_ / 2,
								pipewidth_, Coordinates[i + 1].Y - (Coordinates[i].Y + sliderwidth_ / 2));
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].Y + sliderwidth_ / 2 - sliderinterval_ / 2 + SliderOffy;
									l <= Coordinates[i + 1].Y + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
								{
									fl_begin_line();
									fl_vertex(Coordinates[i].X + pipewidth_ / 2, l);
									fl_vertex(Coordinates[i].X, l + arrowangle_);
									fl_vertex(Coordinates[i].X - pipewidth_ / 2, l);
									fl_end_line();
								}

							}
							fl_pop_clip();
						}
						else
						{
							GraphicDrawHandle::PushClip(Coordinates[i].X - pipewidth_ / 2, Coordinates[i].Y + sliderwidth_ / 2,
								pipewidth_, Coordinates[i + 1].Y - sliderwidth_ / 2 - (Coordinates[i].Y + sliderwidth_ / 2));
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].Y + sliderwidth_ / 2 - sliderinterval_ / 2 + SliderOffy;
									l <= Coordinates[i + 1].Y - sliderwidth_ / 2 + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
								{
									fl_begin_line();
									fl_vertex(Coordinates[i].X + pipewidth_ / 2, l);
									fl_vertex(Coordinates[i].X, l + arrowangle_);
									fl_vertex(Coordinates[i].X - pipewidth_ / 2, l);
									fl_end_line();
								}

							}
							fl_pop_clip();
						}
					}
					else
					{
						if (i == 0)
						{
							if (i == pointnum_ - 2)
							{
								GraphicDrawHandle::PushClip(Coordinates[i].X - pipewidth_ / 2, Coordinates[i].Y,
									pipewidth_, Coordinates[i + 1].Y  - (Coordinates[i].Y));
								{
									fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
									for (int l = Coordinates[i].Y - sliderinterval_ / 2 + SliderOffy;
										l <= Coordinates[i + 1].Y + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
										fl_line(Coordinates[i].X + pipewidth_ / 2, l, Coordinates[i].X - pipewidth_ / 2, l);

								}
								fl_pop_clip();
							}
							else
							{
								GraphicDrawHandle::PushClip(Coordinates[i].X - pipewidth_ / 2, Coordinates[i].Y,
									pipewidth_, Coordinates[i + 1].Y - sliderwidth_ / 2 - (Coordinates[i].Y));
								{
									fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
									for (int l = Coordinates[i].Y - sliderinterval_ / 2 + SliderOffy;
										l <= Coordinates[i + 1].Y - sliderwidth_ / 2 + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
										fl_line(Coordinates[i].X + pipewidth_ / 2, l, Coordinates[i].X - pipewidth_ / 2, l);
								}
								fl_pop_clip();
							}
						}
						else if (i == pointnum_ - 2)
						{
							GraphicDrawHandle::PushClip(Coordinates[i].X - pipewidth_ / 2, Coordinates[i].Y + sliderwidth_ / 2,
								pipewidth_, Coordinates[i + 1].Y - (Coordinates[i].Y + sliderwidth_ / 2));
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].Y + sliderwidth_ / 2 - sliderinterval_ / 2 + SliderOffy;
									l <= Coordinates[i + 1].Y + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
									fl_line(Coordinates[i].X + pipewidth_ / 2, l, Coordinates[i].X - pipewidth_ / 2, l);
							}
							fl_pop_clip();
						}
						else
						{
							GraphicDrawHandle::PushClip(Coordinates[i].X - pipewidth_ / 2, Coordinates[i].Y + sliderwidth_ / 2,
								pipewidth_, Coordinates[i + 1].Y - sliderwidth_ / 2 - (Coordinates[i].Y + sliderwidth_ / 2));
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].Y + sliderwidth_ / 2 - sliderinterval_ / 2 + SliderOffy;
									l <= Coordinates[i + 1].Y - sliderwidth_ / 2 + sliderinterval_ / 2; l = l + sliderlength_ + sliderinterval_)
									fl_line(Coordinates[i].X + pipewidth_ / 2, l, Coordinates[i].X - pipewidth_ / 2, l);
							}
							fl_pop_clip();
						}
					}
				}
				else if (Coordinates[i].Y > Coordinates[i + 1].Y)	//向上竖
				{
					fl_color(active() ? bordercolor_ : fl_inactive(bordercolor_));
					fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, borderwidth_ + pipewidth_);
					fl_line(Coordinates[i].X, Coordinates[i].Y - (borderwidth_ + pipewidth_) / 2, Coordinates[i + 1].X, Coordinates[i + 1].Y);
					fl_color(active() ? pipecolor_ : fl_inactive(pipecolor_));
					fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, pipewidth_);
					fl_line(Coordinates[i].X, Coordinates[i].Y - (pipewidth_) / 2, Coordinates[i + 1].X, Coordinates[i + 1].Y);
					fl_color(active() ? Slidercolor : fl_inactive(Slidercolor));
					fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderwidth_);
					fl_line(Coordinates[i].X, Coordinates[i].Y - sliderwidth_ / 2, Coordinates[i + 1].X, Coordinates[i + 1].Y);
					fl_color(active() ? pipecolor_ : fl_inactive(pipecolor_));
					if (model->FluidConfig.SliderParam.SliderType) {
						if (i == 0)
						{
							if (i == pointnum_ - 2)
							{
								GraphicDrawHandle::PushClip(Coordinates[i + 1].X - pipewidth_ / 2, Coordinates[i + 1].Y,
									pipewidth_, Coordinates[i].Y - (Coordinates[i + 1].Y));
								{
									fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
									for (int l = Coordinates[i].Y + sliderinterval_ / 2 - SliderOffy;
										l > Coordinates[i + 1].Y - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
									{
										fl_begin_line();
										fl_vertex(Coordinates[i].X + pipewidth_ / 2, l);
										fl_vertex(Coordinates[i].X, l - arrowangle_);
										fl_vertex(Coordinates[i].X - pipewidth_ / 2, l);
										fl_end_line();
									}
								}
								fl_pop_clip();
							}
							else
							{
								GraphicDrawHandle::PushClip(Coordinates[i + 1].X - pipewidth_ / 2, Coordinates[i + 1].Y + sliderwidth_ / 2,
									pipewidth_, Coordinates[i].Y - (Coordinates[i + 1].Y + sliderwidth_ / 2));
								{
									fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
									for (int l = Coordinates[i].Y + sliderinterval_ / 2 - SliderOffy;
										l > Coordinates[i + 1].Y + sliderwidth_ / 2 - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
									{
										fl_begin_line();
										fl_vertex(Coordinates[i].X + pipewidth_ / 2, l);
										fl_vertex(Coordinates[i].X, l - arrowangle_);
										fl_vertex(Coordinates[i].X - pipewidth_ / 2, l);
										fl_end_line();
									}
								}
								fl_pop_clip();
							}
						}
						else if (i == pointnum_ - 2)
						{
							GraphicDrawHandle::PushClip(Coordinates[i + 1].X - pipewidth_ / 2, Coordinates[i + 1].Y,
								pipewidth_, Coordinates[i].Y - sliderwidth_ / 2 - (Coordinates[i + 1].Y));
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].Y - sliderwidth_ / 2 + sliderinterval_ / 2 - SliderOffy;
									l > Coordinates[i + 1].Y - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
								{
									fl_begin_line();
									fl_vertex(Coordinates[i].X + pipewidth_ / 2, l);
									fl_vertex(Coordinates[i].X, l - arrowangle_);
									fl_vertex(Coordinates[i].X - pipewidth_ / 2, l);
									fl_end_line();
								}
							}
							fl_pop_clip();
						}
						else
						{
							GraphicDrawHandle::PushClip(Coordinates[i + 1].X - pipewidth_ / 2, Coordinates[i + 1].Y + sliderwidth_ / 2,
								pipewidth_, Coordinates[i].Y - sliderwidth_ / 2 - (Coordinates[i + 1].Y + sliderwidth_ / 2));
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].Y - sliderwidth_ / 2 + sliderinterval_ / 2 - SliderOffy;
									l > Coordinates[i + 1].Y + sliderwidth_ / 2 - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
								{
									fl_begin_line();
									fl_vertex(Coordinates[i].X + pipewidth_ / 2, l);
									fl_vertex(Coordinates[i].X, l - arrowangle_);
									fl_vertex(Coordinates[i].X - pipewidth_ / 2, l);
									fl_end_line();
								}
							}
							fl_pop_clip();
						}
					}
					else
					{
						if (i == 0)
						{
							if (i == pointnum_ - 2)
							{
								GraphicDrawHandle::PushClip(Coordinates[i + 1].X - pipewidth_ / 2, Coordinates[i + 1].Y,
									pipewidth_, Coordinates[i].Y - (Coordinates[i + 1].Y));
								{
									fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
									for (int l = Coordinates[i].Y + sliderinterval_ / 2 - SliderOffy;
										l > Coordinates[i + 1].Y - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
										fl_line(Coordinates[i].X + pipewidth_ / 2, l, Coordinates[i].X - pipewidth_ / 2, l);
								}
								fl_pop_clip();
							}
							else
							{
								GraphicDrawHandle::PushClip(Coordinates[i + 1].X - pipewidth_ / 2, Coordinates[i + 1].Y + sliderwidth_ / 2,
									pipewidth_, Coordinates[i].Y - (Coordinates[i + 1].Y + sliderwidth_ / 2));
								{
									fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
									for (int l = Coordinates[i].Y + sliderinterval_ / 2 - SliderOffy;
										l > Coordinates[i + 1].Y + sliderwidth_ / 2 - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
										fl_line(Coordinates[i].X + pipewidth_ / 2, l, Coordinates[i].X - pipewidth_ / 2, l);
								}
								fl_pop_clip();
							}
						}
						else if (i == pointnum_ - 2)
						{
							GraphicDrawHandle::PushClip(Coordinates[i + 1].X - pipewidth_ / 2, Coordinates[i + 1].Y,
								pipewidth_, Coordinates[i].Y - sliderwidth_ / 2 - (Coordinates[i + 1].Y));
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].Y - sliderwidth_ / 2 + sliderinterval_ / 2 - SliderOffy;
									l > Coordinates[i + 1].Y - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
									fl_line(Coordinates[i].X + pipewidth_ / 2, l, Coordinates[i].X - pipewidth_ / 2, l);
							}
							fl_pop_clip();
						}
						else
						{
							GraphicDrawHandle::PushClip(Coordinates[i + 1].X - pipewidth_ / 2, Coordinates[i + 1].Y + sliderwidth_ / 2,
								pipewidth_, Coordinates[i].Y - sliderwidth_ / 2 - (Coordinates[i + 1].Y + sliderwidth_ / 2));
							{
								fl_line_style(FL_CAP_FLAT | FL_JOIN_MITER, sliderinterval_);
								for (int l = Coordinates[i].Y - sliderwidth_ / 2 + sliderinterval_ / 2 - SliderOffy;
									l > Coordinates[i + 1].Y + sliderwidth_ / 2 - sliderinterval_ / 2; l = l - sliderlength_ - sliderinterval_)
									fl_line(Coordinates[i].X + pipewidth_ / 2, l, Coordinates[i].X - pipewidth_ / 2, l);
							}
							fl_pop_clip();
						}
					}
				}
			}
		}

	}
}
