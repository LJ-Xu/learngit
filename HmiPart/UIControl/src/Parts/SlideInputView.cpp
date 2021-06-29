/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SlideInputView.cpp
 * Author   : qiaodan
 * Date     : 2020-12-17
 * Descript : 绘制滑动输入控件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GraphicDrawHandle.h"
#include "ResourceService.h"
#include "stdafx.h"
#include "SlideInputView.h"
#include "UIComm.h"
#include "FL/fl_draw.H"
#include "SlideInputModel.h"
#include "NoticesUtility.h"
#include "SlideInputControl.h"
#include "UIData.h"
#include "PermUtility.h"
#include "HMIWindow.h"
#include "OperatorGControl.h"
#include "SysSetApi.h"
namespace UI
{
	SlideInputView::SlideInputView(int X, int Y, int W, int H, const char* l) : HMIBaseSlider(X, Y, W, H, l) { }
	SlideInputView::~SlideInputView() {}

	void SlideInputView::SetReadVar()
	{
		shared_ptr<SlideInputControl> ctrl = BaseView.GetControl<SlideInputControl>();
		shared_ptr<SlideInputModel> model = BaseView.GetModel<SlideInputModel>();
		DOUBLE slidevalue = (DOUBLE)value();
		if (model->SlideInputConfig.IsRecord)
			OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->SlideInputConfig.CtrlName, Storage::OA_SET,
				model->SlideInputConfig.ReadVar, UIData::Number<double>(model->SlideInputConfig.ReadVar), slidevalue);
		UIData::Number<DOUBLE>(model->SlideInputConfig.ReadVar, slidevalue);
		/*写入后通知*/
		NoticesUtility::NoticeWrite(model->SlideInputConfig.ResAft);
		//int slidevalue = value();
		//printf("value = %d\n", slidevalue);
	}
	bool SlideInputView::HandleOperatePush()
	{
		shared_ptr<SlideInputModel> model = BaseView.GetModel<SlideInputModel>();
		shared_ptr<SlideInputControl> ctrl = BaseView.GetControl<SlideInputControl>();
		if ((model->SlideInputConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->SlideInputConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			if (model->SlideInputConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->SlideInputConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->SlideInputConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		return true;
	}
	int SlideInputView::handle(int event)
	{
		if (event == FL_PUSH && Fl::visible_focus()) {
			Fl::focus(this);
			redraw();
		}
		return handle(event,0, 0, 0, 0);
	}
	int SlideInputView::handle(int event,int x, int y, int w, int h) {
		shared_ptr<SlideInputModel> model = BaseView.GetModel<SlideInputModel>();
		switch (event) {
		case FL_PUSH: {		
			SysSetApi::TriggerBeep();
			char dataflag = DataApi::GetDataFlag(model->SlideInputConfig.ReadVar);
			if (dataflag == 0)		return 1;
			Fl_Widget_Tracker wp(this);
			if (!Fl::event_inside(model->SlideInputConfig.SlideRailPos.X + model->SlideInputConfig.OffX,
								  model->SlideInputConfig.SlideRailPos.Y + model->SlideInputConfig.OffY,
								  model->SlideInputConfig.SlideRailWidth, 
								  model->SlideInputConfig.SlideRailHeight)) 
				return 0;
			if(!HandleOperatePush())
				return 1;
			//写入前通知
			NoticesUtility::NoticeWrite(model->SlideInputConfig.ResBef);
			if (model->SlideInputConfig.UseIncDecPerClick)
			{
				double val;
				if (minimum() == maximum())
					val = 0.5;
				else {
					val = (value() - minimum()) / (maximum() - minimum());
					if (val > 1.0) val = 1.0;
					else if (val < 0.0) val = 0.0;
				}
			
				int ww = (horizontal() ? model->SlideInputConfig.SlideRailWidth : model->SlideInputConfig.SlideRailHeight);
				int mx = (horizontal() ? Fl::event_x() - (model->SlideInputConfig.SlideRailPos.X + model->SlideInputConfig.OffX) :
					Fl::event_y() - (model->SlideInputConfig.SlideRailPos.Y + model->SlideInputConfig.OffY));
			
				int T = (horizontal() ? model->SlideInputConfig.SlideRailHeight : model->SlideInputConfig.SlideRailWidth) / 2 + 1;
				//if (slideW_ < T) slideW_ = T;
				int xx = int(val * (ww - slideW_) + .5) + slideW_ / 2;

				handle_push();
				if (wp.deleted()) return 1;
				if (mx > xx)
					handle_drag(clamp(increment(value(), 1)));
				else if (mx < xx)
					handle_drag(clamp(increment(value(), -1)));
				handle_release();
				return 1;
			}
			handle_push();
			if (wp.deleted()) return 1;
		}
		case FL_DRAG: {
			char dataflag = DataApi::GetDataFlag(model->SlideInputConfig.ReadVar);
			if (dataflag == 0)		return 1;
			double val;
			if (minimum() == maximum())
				val = 0.5;
			else {
				val = (value() - minimum()) / (maximum() - minimum());
				if (val > 1.0) val = 1.0;
				else if (val < 0.0) val = 0.0;
			}

			int ww = (horizontal() ? model->SlideInputConfig.SlideRailWidth : model->SlideInputConfig.SlideRailHeight);
			int mx = (horizontal() ? Fl::event_x() - (model->SlideInputConfig.SlideRailPos.X + model->SlideInputConfig.OffX) :
				Fl::event_y() - (model->SlideInputConfig.SlideRailPos.Y + model->SlideInputConfig.OffY));

			if (slideW_ >= ww) return 0;

			static int offcenter;
			int T = (horizontal() ? model->SlideInputConfig.SlideRailHeight : model->SlideInputConfig.SlideRailWidth) / 2 + 1;
			//if (slideW_ < T) slideW_ = T;
			if (event == FL_PUSH) {
				int xx = int(val * (ww - slideW_) + .5);
				offcenter = mx - xx;
				if (offcenter < 0) offcenter = 0;
				else if (offcenter > slideW_) offcenter = slideW_;
				else return 1;
			}

			int xx = mx - offcenter;
			double v = 0;
			char tryAgain = 1;
			while (tryAgain)
			{
				tryAgain = 0;
				if (xx < 0) {
					xx = 0;
					offcenter = mx; if (offcenter < 0) offcenter = 0;
				}
				else if (xx > (ww - slideW_)) {
					xx = ww - slideW_;
					offcenter = mx - xx; if (offcenter > slideW_) offcenter = slideW_;
				}

				// v = round(xx*(maximum()-minimum())/(ww-S) + minimum());
				v = xx * (maximum() - minimum()) / (ww - slideW_) + minimum();
				if (event == FL_PUSH && v == value()) {
					offcenter = slideW_ / 2;
					event = FL_DRAG;
					tryAgain = 1;
				}
			}
			
			handle_drag(clamp(v));		
			/*如果是实时显示，将数值写入到ReadVar,否则不处理*/
			if (model->SlideInputConfig.UseChangeInput)
				SetReadVar();
		} return 1;
		case FL_RELEASE:	
		{
			char dataflag = DataApi::GetDataFlag(model->SlideInputConfig.ReadVar);
			if (dataflag == 0)		return 1;
			/*标志FL_WHEN_RELEASE释放后调用callback将数值写入到寄存器*/
			handle_release();
			/*将数值写入寄存器*/
			SetReadVar();
			PermUtility::QuitLimitPerm(model->SlideInputConfig.Perm);
			return 1;
		}
		case FL_FOCUS:
		case FL_UNFOCUS:
			if (Fl::visible_focus()) {
				redraw();
				return 1;
			}
			else return 0;
		case FL_ENTER:
		case FL_LEAVE:
			return 1;
		default:
			return 0;
		}
	}

	/*转换数据显示格式*/
	string SlideInputView::ChangeDisplayFormat(double value, int mode)
	{
		shared_ptr<SlideInputModel> model = BaseView.GetModel<SlideInputModel>();
		string data = to_string(value);
		string intString, floatString;
		int pos = data.find(".");
		if (pos == 0)	//小数点在头部
		{
			intString = "0";
			floatString = data.substr(pos + 1, data.size());
		}
		else if (pos == -1)
		{
			intString = data;
			floatString = "";
		}
		else
		{
			intString = data.substr(0, pos);
			floatString = data.substr(pos + 1, data.size());
		}
		//去除头部多余0和增加小数点前0
		int f = atoi(intString.c_str());
		intString = to_string(f);
		if (floatString.size() < (unsigned)model->SlideInputConfig.Bct)
		{
			unsigned size = (unsigned)model->SlideInputConfig.Bct - floatString.size();
			for (unsigned i = 0; i < size;i++)
				floatString += "0";
		}
		else
			floatString = floatString.substr(0, model->SlideInputConfig.Bct);
		if (intString.size() > (unsigned)model->SlideInputConfig.Act)
		{
			string maxString;
			for (size_t i = 0; i < (size_t)model->SlideInputConfig.Act; i++)
				maxString += "*";
			return maxString;
		}
		if (model->SlideInputConfig.Bct)
		{
			if (mode) //百分比
				return intString + "." + floatString + "%";
			else
				return intString + "." + floatString;
		}
		else
		{
			if (mode)
				return intString + "%";
			else
				return intString;
		}
	}
	void SlideInputView::DrawYScale()
	{
		shared_ptr<SlideInputModel> model = BaseView.GetModel<SlideInputModel>();
		fl_color(active() ? axisColor_ : fl_inactive(axisColor_));    //设置刻度线的颜色   
		fl_line_style(model->SlideInputConfig.ScaleLineStyle.Type,
			model->SlideInputConfig.ScaleLineStyle.Weight);	//设置刻度线样式
		
		//绘制刻度
		double spaceInterval;
		/*绘制Y轴刻度*/
		spaceInterval = (double)axislength_ / (double)(model->SlideInputConfig.PrimaryScaleNum * model->SlideInputConfig.SecondaryScaleNum); //刻度间隔
		//确定轴线位置绘制轴线
		if (model->SlideInputConfig.ShowAxis)
			fl_line(axispos_.X + model->SlideInputConfig.OffX,
				axispos_.Y + model->SlideInputConfig.OffY,
				axispos_.X + model->SlideInputConfig.OffX,
				axispos_.Y + model->SlideInputConfig.OffY + (int)(spaceInterval * (double)(model->SlideInputConfig.PrimaryScaleNum * model->SlideInputConfig.SecondaryScaleNum)));

		for (int i = 0; i < model->SlideInputConfig.PrimaryScaleNum * model->SlideInputConfig.SecondaryScaleNum + 1; i++)	//绘制刻度线	
		{
			if (i % model->SlideInputConfig.SecondaryScaleNum == 0)         //主刻度
			{
				fl_begin_line();
				fl_vertex((double)axispos_.X + (double)model->SlideInputConfig.OffX,
					(double)axispos_.Y + (spaceInterval * (double)i) + (double)model->SlideInputConfig.OffY);
				fl_vertex((double)axispos_.X + + (double)primaryScaleLen_ + (double)model->SlideInputConfig.OffX,
					(double)axispos_.Y + (spaceInterval * (double)i) + (double)model->SlideInputConfig.OffY);
				fl_end_line();
				//fl_line(axispos_.X + model->SlideInputConfig.OffX,
				//	axispos_.Y + (int)(spaceInterval * i) + model->SlideInputConfig.OffY,
				//	axispos_.X + primaryScaleLen_ + model->SlideInputConfig.OffX,
				//	axispos_.Y + (int)(spaceInterval * i) + model->SlideInputConfig.OffY);
			}
			else
			{
				fl_begin_line();
				fl_vertex((double)axispos_.X + (double)model->SlideInputConfig.OffX,
					(double)axispos_.Y + (spaceInterval * (double)i) + (double)model->SlideInputConfig.OffY);
				fl_vertex((double)axispos_.X +(double)secondScaleLen_ + (double)model->SlideInputConfig.OffX,
					(double)axispos_.Y + (spaceInterval * (double)i) + (double)model->SlideInputConfig.OffY);
				fl_end_line();
				//fl_line(axispos_.X + model->SlideInputConfig.OffX,
				//	axispos_.Y + (int)(spaceInterval * i) + model->SlideInputConfig.OffY,
				//	axispos_.X + secondScaleLen_ + model->SlideInputConfig.OffX,
				//	axispos_.Y + (int)(spaceInterval * i) + model->SlideInputConfig.OffY);
			}
		}
		/*绘制坐标标注*/
		if (model->SlideInputConfig.ShowMark)
		{
			/*设置字体*/
			fl_font(scalefont_, scalesize_);
			/*设置字体颜色*/
			fl_color(active() ? scalecolor_ : fl_inactive(scalecolor_));
			double numInterval = (double)(model->SlideInputConfig.AttrMax - model->SlideInputConfig.AttrMin) / (float)model->SlideInputConfig.PrimaryScaleNum;
			for (size_t i = 0, j = model->SlideInputConfig.ScaleInfo.size() - 1;
				i < model->SlideInputConfig.ScaleInfo.size(); i++, j--)
			{
				double num = numInterval * (double)i + (double)model->SlideInputConfig.AttrMin;
				if (num > model->SlideInputConfig.AttrMax)
					num = model->SlideInputConfig.AttrMax;
				if (model->SlideInputConfig.Dir == Project::SlideDir::SildeDown)
				{
					model->SlideInputConfig.ScaleInfo[j].ScaleContent = ChangeDisplayFormat(num, 0);
					fl_draw(model->SlideInputConfig.ScaleInfo[j].ScaleContent.data(),
						model->SlideInputConfig.ScaleInfo[j].Coordinate.X + model->SlideInputConfig.OffX,
						model->SlideInputConfig.ScaleInfo[j].Coordinate.Y + model->SlideInputConfig.OffY,
						model->SlideInputConfig.ScaleInfo[j].Width, model->SlideInputConfig.ScaleInfo[j].Height, FL_ALIGN_CENTER);
				}
				else
				{
					model->SlideInputConfig.ScaleInfo[i].ScaleContent = ChangeDisplayFormat(num, 0);
					fl_draw(model->SlideInputConfig.ScaleInfo[i].ScaleContent.data(),
						model->SlideInputConfig.ScaleInfo[i].Coordinate.X + model->SlideInputConfig.OffX,
						model->SlideInputConfig.ScaleInfo[i].Coordinate.Y + model->SlideInputConfig.OffY,
						model->SlideInputConfig.ScaleInfo[i].Width, model->SlideInputConfig.ScaleInfo[i].Height, FL_ALIGN_CENTER);

				}
			}
		}
		else if (model->SlideInputConfig.ShowPer)
		{
			/*设置字体*/
			fl_font(scalefont_, scalesize_);
			/*设置字体颜色*/
			fl_color(active() ? scalecolor_ : fl_inactive(scalecolor_));
			float numInterval = (float)100.0 / (float)model->SlideInputConfig.PrimaryScaleNum;
			for (size_t i = 0, j = model->SlideInputConfig.ScaleInfo.size() - 1;
				i < model->SlideInputConfig.ScaleInfo.size(); i++, j--)
			{
				float num = numInterval * (float)i + (float)model->SlideInputConfig.AttrMin;
				if (num > 100)	num = 100;
				if (model->SlideInputConfig.Dir == Project::SlideDir::SildeDown)
				{
					model->SlideInputConfig.ScaleInfo[j].ScaleContent = ChangeDisplayFormat(num, 1);
					fl_draw(model->SlideInputConfig.ScaleInfo[j].ScaleContent.data(),
						model->SlideInputConfig.ScaleInfo[j].Coordinate.X + model->SlideInputConfig.OffX,
						model->SlideInputConfig.ScaleInfo[j].Coordinate.Y + model->SlideInputConfig.OffY,
						model->SlideInputConfig.ScaleInfo[j].Width, model->SlideInputConfig.ScaleInfo[j].Height, FL_ALIGN_CENTER);
				}
				else
				{
					model->SlideInputConfig.ScaleInfo[i].ScaleContent = ChangeDisplayFormat(num, 1);
					fl_draw(model->SlideInputConfig.ScaleInfo[i].ScaleContent.data(),
						model->SlideInputConfig.ScaleInfo[i].Coordinate.X + model->SlideInputConfig.OffX,
						model->SlideInputConfig.ScaleInfo[i].Coordinate.Y + model->SlideInputConfig.OffY,
						model->SlideInputConfig.ScaleInfo[i].Width, model->SlideInputConfig.ScaleInfo[i].Height, FL_ALIGN_CENTER);
				}
			}
		}
	}
	void SlideInputView::DrawXScale()
	{
		shared_ptr<SlideInputModel> model = BaseView.GetModel<SlideInputModel>();
		fl_color(active() ? axisColor_ : fl_inactive(axisColor_));    //设置刻度线的颜色   
		fl_line_style(model->SlideInputConfig.ScaleLineStyle.Type,
			model->SlideInputConfig.ScaleLineStyle.Weight);	//设置刻度线样式
		//绘制刻度
		double spaceInterval;
		/*绘制X轴刻度*/
		spaceInterval = (double)axislength_ / ((double)model->SlideInputConfig.PrimaryScaleNum * (double)model->SlideInputConfig.SecondaryScaleNum); //刻度间隔
		//确定轴线位置绘制轴线
		if (model->SlideInputConfig.ShowAxis)
			fl_line(axispos_.X + model->SlideInputConfig.OffX,
				axispos_.Y + model->SlideInputConfig.OffY,
				axispos_.X + model->SlideInputConfig.OffX + (int)(spaceInterval * (double)(model->SlideInputConfig.PrimaryScaleNum * model->SlideInputConfig.SecondaryScaleNum)),
				axispos_.Y + model->SlideInputConfig.OffY);
		for (int i = 0; i < model->SlideInputConfig.PrimaryScaleNum * model->SlideInputConfig.SecondaryScaleNum + 1; i++)	//绘制刻度线	
		{
			if (i % model->SlideInputConfig.SecondaryScaleNum == 0)         //主刻度
			{
				fl_begin_line();
				fl_vertex((double)axispos_.X + (spaceInterval * (double)i) + (double)model->SlideInputConfig.OffX,
					(double)axispos_.Y + (double)model->SlideInputConfig.OffY);
				fl_vertex((double)axispos_.X + (spaceInterval * (double)i) + (double)model->SlideInputConfig.OffX,
					(double)axispos_.Y + (double)primaryScaleLen_ + (double)model->SlideInputConfig.OffY);
				fl_end_line();
				//fl_line(axispos_.X + (int)(spaceInterval * i) + model->SlideInputConfig.OffX,
				//	axispos_.Y + model->SlideInputConfig.OffY,
				//	axispos_.X + (int)(spaceInterval * i) + model->SlideInputConfig.OffX,
				//	axispos_.Y + primaryScaleLen_ + model->SlideInputConfig.OffY);

			}
			else
			{
				fl_begin_line();
				fl_vertex((double)axispos_.X + (spaceInterval * (double)i) + (double)model->SlideInputConfig.OffX,
					(double)axispos_.Y + (double)model->SlideInputConfig.OffY);
				fl_vertex((double)axispos_.X + (spaceInterval * (double)i) + (double)model->SlideInputConfig.OffX,
					(double)axispos_.Y + (double)secondScaleLen_ + (double)model->SlideInputConfig.OffY);
				fl_end_line();
				//fl_line(axispos_.X + (int)(spaceInterval * i) + model->SlideInputConfig.OffX,
				//	axispos_.Y + model->SlideInputConfig.OffY,
				//	axispos_.X + (int)(spaceInterval * i) + model->SlideInputConfig.OffX,
				//	axispos_.Y + secondScaleLen_ + model->SlideInputConfig.OffY);
			}

		}
		/*绘制坐标标注*/
		if (model->SlideInputConfig.ShowMark)
		{
			/*设置字体*/
			fl_font(scalefont_, scalesize_);
			/*设置字体颜色*/
			fl_color(active() ? scalecolor_ : fl_inactive(scalecolor_));
			double numInterval = (double)(model->SlideInputConfig.AttrMax - model->SlideInputConfig.AttrMin) / (float)model->SlideInputConfig.PrimaryScaleNum;
			for (size_t i = 0, j = model->SlideInputConfig.ScaleInfo.size() - 1;
				i < model->SlideInputConfig.ScaleInfo.size(); i++, j--) 
			{
				double num = numInterval * (double)i + (double)model->SlideInputConfig.AttrMin;
				if (num > model->SlideInputConfig.AttrMax)
					num = model->SlideInputConfig.AttrMax;
				if (model->SlideInputConfig.Dir == Project::SlideDir::SildeLeft)
				{
					model->SlideInputConfig.ScaleInfo[j].ScaleContent = ChangeDisplayFormat(num, 0);
					fl_draw(model->SlideInputConfig.ScaleInfo[j].ScaleContent.data(),
						model->SlideInputConfig.ScaleInfo[j].Coordinate.X + model->SlideInputConfig.OffX,
						model->SlideInputConfig.ScaleInfo[j].Coordinate.Y + model->SlideInputConfig.OffY,
						model->SlideInputConfig.ScaleInfo[j].Width, model->SlideInputConfig.ScaleInfo[j].Height, FL_ALIGN_CENTER);
				}
				else
				{
					model->SlideInputConfig.ScaleInfo[i].ScaleContent = ChangeDisplayFormat(num, 0);
					fl_draw(model->SlideInputConfig.ScaleInfo[i].ScaleContent.data(),
						model->SlideInputConfig.ScaleInfo[i].Coordinate.X + model->SlideInputConfig.OffX,
						model->SlideInputConfig.ScaleInfo[i].Coordinate.Y + model->SlideInputConfig.OffY,
						model->SlideInputConfig.ScaleInfo[i].Width, model->SlideInputConfig.ScaleInfo[i].Height, FL_ALIGN_CENTER);
				}
			}

		}
		else if (model->SlideInputConfig.ShowPer)
		{
			/*设置字体*/
			fl_font(scalefont_, scalesize_);
			/*设置字体颜色*/
			fl_color(active() ? scalecolor_ : fl_inactive(scalecolor_));
			double numInterval = (double)100.0 / (double)model->SlideInputConfig.PrimaryScaleNum;
			for (size_t i = 0, j = model->SlideInputConfig.ScaleInfo.size() - 1; 
				i < model->SlideInputConfig.ScaleInfo.size(); i++, j --)
			{
				double num = numInterval * (double)i + (double)model->SlideInputConfig.AttrMin;
				if (num > 100.0)	num = 100.0;
				if (model->SlideInputConfig.Dir == Project::SlideDir::SildeLeft)
				{
					model->SlideInputConfig.ScaleInfo[j].ScaleContent = ChangeDisplayFormat(num, 1);
					fl_draw(model->SlideInputConfig.ScaleInfo[j].ScaleContent.data(),
						model->SlideInputConfig.ScaleInfo[j].Coordinate.X + model->SlideInputConfig.OffX,
						model->SlideInputConfig.ScaleInfo[j].Coordinate.Y + model->SlideInputConfig.OffY,
						model->SlideInputConfig.ScaleInfo[j].Width, model->SlideInputConfig.ScaleInfo[j].Height, FL_ALIGN_CENTER);
				}
				else
				{
					model->SlideInputConfig.ScaleInfo[i].ScaleContent = ChangeDisplayFormat(num, 1);
					fl_draw(model->SlideInputConfig.ScaleInfo[i].ScaleContent.data(),
						model->SlideInputConfig.ScaleInfo[i].Coordinate.X + model->SlideInputConfig.OffX,
						model->SlideInputConfig.ScaleInfo[i].Coordinate.Y + model->SlideInputConfig.OffY,
						model->SlideInputConfig.ScaleInfo[i].Width, model->SlideInputConfig.ScaleInfo[i].Height, FL_ALIGN_CENTER);
				}
			}
		}
	}

	void SlideInputView::DrawScale()
	{
		shared_ptr<SlideInputModel> model = BaseView.GetModel<SlideInputModel>();
		if (model->SlideInputConfig.Dir == Project::SlideDir::SildeDown ||
			model->SlideInputConfig.Dir == Project::SlideDir::SildeUp)
			DrawYScale();
		else
			DrawXScale();
	}

	void SlideInputView::InitDraw()
	{
		shared_ptr<SlideInputModel> model = BaseView.GetModel<SlideInputModel>();
		slideRailBackColor_ = fl_rgb_color(RGBColor(model->SlideInputConfig.SlideRailBackColor));
		slideRailFrameColor_ = fl_rgb_color(RGBColor(model->SlideInputConfig.SlideRailBorderColor));
		slideBackColor_ = fl_rgb_color(RGBColor(model->SlideInputConfig.SlideBackColor));
		slideFrameColor_ = fl_rgb_color(RGBColor(model->SlideInputConfig.SlideBorderColor));

		if (model->SlideInputConfig.Dir == Project::SlideDir::SildeUp ||
			model->SlideInputConfig.Dir == Project::SlideDir::SildeLeft)
		{
			minimum(model->SlideInputConfig.AttrMax);
			maximum(model->SlideInputConfig.AttrMin);
		}
		else
		{
			minimum(model->SlideInputConfig.AttrMin);
			maximum(model->SlideInputConfig.AttrMax);
		}

		if (model->SlideInputConfig.UseIncDecPerClick)
			step(model->SlideInputConfig.IncDecPerClickScale * model->SlideInputConfig.MinScale);
		else
			step(model->SlideInputConfig.MinScale);

		axispos_.X = model->SlideInputConfig.AxisPos.X;
		axispos_.Y = model->SlideInputConfig.AxisPos.Y;
		/*设置垂直水平功能*/
		if (model->SlideInputConfig.Dir == Project::SlideDir::SildeDown ||
			model->SlideInputConfig.Dir == Project::SlideDir::SildeUp)
		{
			type(FL_VERTICAL);		//垂直
			slideW_ = (horizontal() ? model->SlideInputConfig.SlideWidth : model->SlideInputConfig.SlideHeight);
			axislength_ = model->SlideInputConfig.SlideRailHeight - slideW_;
			if (model->SlideInputConfig.ScalePos == 0)		//在滑轨左方
			{
				//axispos_.X = model->SlideInputConfig.SlideRailPos.X - 2;
				//axispos_.Y = model->SlideInputConfig.SlideRailPos.Y + slideW_ / 2;
				primaryScaleLen_ = -model->SlideInputConfig.PrimaryScaleLen;
				secondScaleLen_ = -model->SlideInputConfig.SecondaryScaleLen;
			}
			else		//在滑轨右方
			{
				//axispos_.X = model->SlideInputConfig.SlideRailPos.X + model->SlideInputConfig.SlideRailWidth + 2;
				//axispos_.Y = model->SlideInputConfig.SlideRailPos.Y + slideW_ / 2;
				primaryScaleLen_ = model->SlideInputConfig.PrimaryScaleLen;
				secondScaleLen_ = model->SlideInputConfig.SecondaryScaleLen;
			}
		}
		else
		{
			type(FL_HORIZONTAL);	//水平
			slideW_ = (horizontal() ? model->SlideInputConfig.SlideWidth : model->SlideInputConfig.SlideHeight);
			axislength_ = model->SlideInputConfig.SlideRailWidth - slideW_;
			if (model->SlideInputConfig.ScalePos == 0)		//在滑轨上方
			{
				//axispos_.X = model->SlideInputConfig.SlideRailPos.X + slideW_/2;
				//axispos_.Y = model->SlideInputConfig.SlideRailPos.Y - 2;
				primaryScaleLen_ = -model->SlideInputConfig.PrimaryScaleLen;
				secondScaleLen_ = -model->SlideInputConfig.SecondaryScaleLen;
			}
			else
			{
				//axispos_.X = model->SlideInputConfig.SlideRailPos.X + slideW_ / 2;
				//axispos_.Y = model->SlideInputConfig.SlideRailPos.Y + model->SlideInputConfig.SlideRailHeight + 2;
				primaryScaleLen_ = model->SlideInputConfig.PrimaryScaleLen;
				secondScaleLen_ = model->SlideInputConfig.SecondaryScaleLen;
			}
		}

		if (model->SlideInputConfig.ShowScale)
		{
			axisColor_ = fl_rgb_color(RGBColor(model->SlideInputConfig.ScaleLineStyle.Color));
			/*设置标注字体*/
			if (model->SlideInputConfig.ShowMark)	//显示数字标志
			{
				scalefont_ = UI::IResourceService::GetFontIdx(model->SlideInputConfig.MarkStyle.Font.Name);
				scalecolor_ = fl_rgb_color(RGBColor(model->SlideInputConfig.MarkStyle.Colors));
				scalesize_ = model->SlideInputConfig.MarkStyle.Font.Size;
			}
			else if (model->SlideInputConfig.ShowPer)
			{
				scalefont_ = UI::IResourceService::GetFontIdx(model->SlideInputConfig.PerStyle.Font.Name);
				scalecolor_ = fl_rgb_color(RGBColor(model->SlideInputConfig.PerStyle.Colors));
				scalesize_ = model->SlideInputConfig.PerStyle.Font.Size;
			}
		}
		value(0);
	}
	
	void SlideInputView::draw()
	{
		shared_ptr<SlideInputModel> model = BaseView.GetModel<SlideInputModel>();
		double val;
		if (minimum() == maximum())
			val = 0.5;
		else {
			val = (value() - minimum()) / (maximum() - minimum());
			if (val > 1.0) val = 1.0;
			else if (val < 0.0) val = 0.0;
		}
		int ww = (horizontal() ? model->SlideInputConfig.SlideRailWidth : model->SlideInputConfig.SlideRailHeight);
		int xx;
		int T = (horizontal() ? model->SlideInputConfig.SlideRailWidth : model->SlideInputConfig.SlideRailHeight) / 2 + 1;
		//if (slideW_ < T) slideW_ = T;
		xx = int(val*(ww - slideW_) + .5);
		
		int xsl, ysl, wsl, hsl;
		if (horizontal()) {
			xsl = model->SlideInputConfig.SlideRailPos.X + xx + model->SlideInputConfig.OffX;
			wsl = slideW_;
			ysl = model->SlideInputConfig.SlideRailPos.Y + model->SlideInputConfig.OffY +
				(model->SlideInputConfig.SlideRailHeight - model->SlideInputConfig.SlideHeight) / 2;
			hsl = model->SlideInputConfig.SlideHeight;
		}
		else {
			ysl = model->SlideInputConfig.SlideRailPos.Y + xx + model->SlideInputConfig.OffY;
			hsl = slideW_;
			xsl = model->SlideInputConfig.SlideRailPos.X + model->SlideInputConfig.OffX +
				(model->SlideInputConfig.SlideRailWidth - model->SlideInputConfig.SlideWidth) / 2;
			wsl = model->SlideInputConfig.SlideWidth;
		}

		///*绘制滑轨*/
		//GraphicDrawHandle::PushClip(model->SlideInputConfig.SlideRailPos.X + model->SlideInputConfig.OffX, 
		//	model->SlideInputConfig.SlideRailPos.Y + model->SlideInputConfig.OffY,
		//	model->SlideInputConfig.SlideRailWidth, model->SlideInputConfig.SlideRailHeight);

		Fl_Image *sliderailimage = nullptr;
		sliderailimage = IResourceService::Ins()->GetImage(model->SlideInputConfig.SlideRailKey.KeyVal);
		if (!sliderailimage) {
			LOG_INFO_("sliderailimage is NULL\n");
			draw_box(FL_FLAT_BOX, model->SlideInputConfig.SlideRailPos.X + model->SlideInputConfig.OffX,
				model->SlideInputConfig.SlideRailPos.Y + model->SlideInputConfig.OffY,
				model->SlideInputConfig.SlideRailWidth, model->SlideInputConfig.SlideRailHeight, slideRailBackColor_);
			fl_rect(model->SlideInputConfig.SlideRailPos.X + model->SlideInputConfig.OffX, 
				model->SlideInputConfig.SlideRailPos.Y + model->SlideInputConfig.OffY,
				model->SlideInputConfig.SlideRailWidth, model->SlideInputConfig.SlideRailHeight, 
				active() ? slideRailFrameColor_ : fl_inactive(slideRailFrameColor_));
		}
		else {
			//GraphicDrawHandle::PushClip(model->SlideInputConfig.SlideRailPos.X + model->SlideInputConfig.OffX, 
			//	model->SlideInputConfig.SlideRailPos.Y + model->SlideInputConfig.OffY,
			//	model->SlideInputConfig.SlideRailWidth, model->SlideInputConfig.SlideRailHeight);
			sliderailimage->draw(model->SlideInputConfig.SlideRailPos.X + model->SlideInputConfig.OffX, 
				model->SlideInputConfig.SlideRailPos.Y + model->SlideInputConfig.OffY);
			//fl_pop_clip();
		}
		//fl_pop_clip();

		/*绘制滑块*/
		if (wsl > 0 && hsl > 0)
		{
			Fl_Image *slideimage = nullptr;
			slideimage = IResourceService::Ins()->GetImage(model->SlideInputConfig.SlideKey.KeyVal);
			if (!slideimage) {
				LOG_INFO_("Slideimage is NULL\n");
				draw_box(FL_FLAT_BOX, xsl, ysl, wsl, hsl, slideBackColor_);
				fl_rect(xsl, ysl, wsl, hsl, active() ? slideFrameColor_ : fl_inactive(slideFrameColor_));
			}
			else {
				//GraphicDrawHandle::PushClip(xsl, ysl, wsl, hsl);
				slideimage->draw(xsl, ysl);
				//fl_pop_clip();
			}
		}
		/*绘制标注*/
		GraphicDrawHandle::PushClip(model->SlideInputConfig.X + model->SlideInputConfig.OffX,
			model->SlideInputConfig.Y + model->SlideInputConfig.OffY,
			model->SlideInputConfig.Width, model->SlideInputConfig.Height);
		if(model->SlideInputConfig.ShowScale)
			DrawScale();
		fl_pop_clip();
	}
}
