/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PieChartView.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : ��ͼ�ؼ�������
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
//#include "stdafx.h"
#include "PieChartView.h"
#include "PieChartModel.h"
#include "PieChartControl.h"
#include "IResourceService.h"
//#include "ResourceService.h"
#include <iostream>
#include <math.h>
#include <iomanip>
//#include "GraphicDrawHandle.h"
#define M_PI       3.14159265358979323846   // pi
namespace UI
{
	PieChartView::PieChartView(int x, int y, int w, int h) : HMIBaseLabel(x, y, w, h) {

	}

	PieChartView::~PieChartView() {

	}

	void PieChartView::InitView(void* p)
	{
		PieChartModel* model = (PieChartModel*)p;
		float wholep = 100.0;
		float lastp = wholep;
		float wholeitg = 0;
		for (int i = 0; i < model->PieChartUnit.Channels.size(); ++i)
		{
			wholeitg += (i+1);
		}
		for (int i = 0; i < model->PieChartUnit.Channels.size(); ++i)
		{
			model->PieChartUnit.Channels[i].Percent = (float)(i+1) / wholeitg;
			/*if (i != model->PieChartUnit.Channels.size() - 1)
			{
				model->PieChartUnit.Channels[i].Percent = (lastp / 2.0f) / wholep;
				lastp /= 2.0f;
			}
			else
				model->PieChartUnit.Channels[i].Percent = lastp/ wholep;*/
		}
	}

	// ���ƿؼ�
	void PieChartView::draw() {
		shared_ptr<PieChartModel> model = BaseView.GetModel<PieChartModel>();
		// ���Ʊ�ͼ����
		Rectangle rect;
		model->GetRect(rect);
		// ��ȡ��ͼ�ܽǶ�
		int totalAngle = model->PieChartUnit.EndAngle - model->PieChartUnit.StartAngle;
		int usedAngle = 0;
		int wSAng = 90, wEAng = 90;
		int startAngle = 0;
		int endAngle=0;
		switch (model->PieChartUnit.Dir)
		{
		case 0:
			wEAng -= model->PieChartUnit.StartAngle;
			wSAng -= model->PieChartUnit.EndAngle;
			startAngle = wEAng;
			endAngle = wSAng;
			break;
		case 1:
			wSAng += model->PieChartUnit.StartAngle;
			wEAng += model->PieChartUnit.EndAngle;
			startAngle = wSAng;
			endAngle = wEAng;
			break;
		}
		vector<std::pair<int, int>>().swap(TxtPosList);
		int fontSize = model->PieChartUnit.TextStyle.Font.Size;
		int fontStyle = UI::IResourceService::GetFontIdx(model->PieChartUnit.TextStyle.Font.Name);
		fl_font(fontStyle, fontSize);
		if (startAngle < 0)
			startAngle += 360;
		// ����ͼ
		for (int i = 0; i < model->PieChartUnit.Channels.size(); ++i)
		{
			// ��ȡ�ǶȰٷֱ�
			//int angle = i == ValueSnapList.size() - 1 ?totalAngle - usedAngle : totalAngle *model->PieChartUnit.Channels[i].Percent;
			int angle = totalAngle * model->PieChartUnit.Channels[i].Percent;
			int endAngle = model->PieChartUnit.Dir == 0 ? startAngle - angle : startAngle + angle;
			/*if (endAngle < 0)
				endAngle += 360;*/
			/*if (endAngle > 360)
				endAngle -= 360;*/
			/*if (startAngle >= 360)
				startAngle -= 360;*/
			// ����������ʼ�Ƕ�
			model->PieChartUnit.Channels[i].StartAngle = startAngle;
			// ����������ֹ�Ƕ�
			model->PieChartUnit.Channels[i].EndAngle = endAngle;
			// ������������
			if (model->PieChartUnit.Mode == 0|| model->PieChartUnit.Channels[i].Percent==0) {
				model->PieChartUnit.Channels[i].text = "";
			} else if (model->PieChartUnit.Mode == 1) {
				string str = to_string(model->PieChartUnit.Channels[i].Val);
				int pidx = str.find_first_of(".");
				if(pidx==-1)
				{
					str.append(".");
					int pd = model->PieChartUnit.Bct;
					while (pd--)
					{
						str.append("0");
					}
				}
				else
				{
					//���С��λ
					int lstDgt = model->PieChartUnit.Bct - (str.size() - 1 - pidx);
					if (lstDgt <= 0)//С��λ������պ�
					{
						str = str.substr(0, pidx + model->PieChartUnit.Bct);
					}
					else //if (lstDgt > 0)//����û������С��λ
					{
						while (lstDgt--)
						{
							str.append("0");
						}
					}
				}
				model->PieChartUnit.Channels[i].text = str;
			}
			else if (model->PieChartUnit.Mode == 2) {
				model->PieChartUnit.Channels[i].text = to_string((int)(model->PieChartUnit.Channels[i].Percent*100.0f)).append("%");
			}
			// ����������ʽ
			DrawPie(model->PieChartUnit.Channels[i], rect, 
					model->PieChartUnit.Dir, 
					model->PieChartUnit.TextStyle);
			startAngle = endAngle;
			usedAngle += angle;
 		}
		// ��ȡ�߿���ɫ
		Fl_Color borderColor = fl_rgb_color(
			model->PieChartUnit.BorderColor.R,
			model->PieChartUnit.BorderColor.G,
			model->PieChartUnit.BorderColor.B);
		//fl_color(borderColor);
		GraphicDrawHandle::Ins()->SetBrushStyle(borderColor, 0, 1, model->PieChartUnit.Alpha);
	 	// ���Ʊ�ͼ�߿�
	 	fl_arc(model->PieChartUnit.X,
	 		   model->PieChartUnit.Y,
	 		   model->PieChartUnit.Width,
	 		   model->PieChartUnit.Height,
	 		   wSAng,
	 		   wEAng);
		// �����ڲ��߿�
		//for (int i = 0; i < model->PieChartUnit.ChannelNum; ++i) 
		for (int i = 0; i < model->PieChartUnit.Channels.size(); ++i)
		{
			double wVal = cos(model->PieChartUnit.Channels[i].StartAngle * M_PI / 180) * (rect.W / 2);
			double hVal = sin(model->PieChartUnit.Channels[i].StartAngle * M_PI / 180) * (rect.H / 2);
			// �������������
			fl_line(rect.X + rect.W / 2, rect.Y + rect.H / 2,
				rect.X + rect.W / 2 + wVal, rect.Y + rect.H / 2 - hVal);
		}
		double wVal = cos(endAngle * M_PI / 180) * (rect.W / 2);
		double hVal = sin(endAngle * M_PI / 180) * (rect.H / 2);
		// �������������
		fl_line(rect.X + rect.W / 2, rect.Y + rect.H / 2,
			rect.X + rect.W / 2 + wVal, rect.Y + rect.H / 2 - hVal);
		// ���ڲ�Բ�뾶��Ϊ0
		if (model->PieChartUnit.CenterRadius > 0) {
			Fl_Color centerColor = fl_rgb_color(
				model->PieChartUnit.CenterColor.R,
				model->PieChartUnit.CenterColor.G,
				model->PieChartUnit.CenterColor.B);
			//fl_color(centerColor);
			GraphicDrawHandle::Ins()->SetBrushStyle(centerColor, 0, 0, model->PieChartUnit.Alpha);
			// �����ڲ�Բ��
			//fl_pie(model->PieChartUnit.X + (model->PieChartUnit.Width - model->PieChartUnit.CenterRadius) / 2,model->PieChartUnit.Y + (model->PieChartUnit.Height - model->PieChartUnit.CenterRadius) /2 ,model->PieChartUnit.CenterRadius, model->PieChartUnit.CenterRadius, 0, 360);
			fl_pie(model->PieChartUnit.X + model->PieChartUnit.Width / 2-model->PieChartUnit.CenterRadius,model->PieChartUnit.Y + model->PieChartUnit.Height / 2 - model->PieChartUnit.CenterRadius,model->PieChartUnit.CenterRadius*2, model->PieChartUnit.CenterRadius*2,
				wSAng,
				wEAng);

			Fl_Color centerEdgeColor = fl_rgb_color(
				model->PieChartUnit.CenterEdgeColor.R,
				model->PieChartUnit.CenterEdgeColor.G,
				model->PieChartUnit.CenterEdgeColor.B);
			//fl_color(centerEdgeColor);
			GraphicDrawHandle::Ins()->SetBrushStyle(centerEdgeColor, 0, 0, model->PieChartUnit.Alpha);
			// �����ڲ��߿�
			fl_arc(model->PieChartUnit.X + model->PieChartUnit.Width / 2 - model->PieChartUnit.CenterRadius,
				model->PieChartUnit.Y + model->PieChartUnit.Height / 2 - model->PieChartUnit.CenterRadius,
				model->PieChartUnit.CenterRadius * 2, model->PieChartUnit.CenterRadius * 2,
				wSAng,
				wEAng);

			if (360 != (int)abs(model->PieChartUnit.EndAngle - model->PieChartUnit.StartAngle))
			{
				double stArc = wSAng * M_PI / 180;
				double edArc = wEAng * M_PI / 180;
				int centerX = model->PieChartUnit.X + model->PieChartUnit.Width / 2;
				int centerY = model->PieChartUnit.Y + model->PieChartUnit.Height / 2;
				int sy = centerY - sin(stArc)*model->PieChartUnit.CenterRadius;
				int sx = centerX + cos(stArc)*model->PieChartUnit.CenterRadius;
				int ey = centerY - sin(edArc)*model->PieChartUnit.CenterRadius;
				int ex = centerX + cos(edArc)*model->PieChartUnit.CenterRadius;

				fl_line(centerX, centerY, sx, sy);
				fl_line(centerX, centerY, ex, ey);
			}
		}

		//��������
		if (model->PieChartUnit.Channels.size()== TxtPosList.size())
		{
			for (int i = 0; i < model->PieChartUnit.Channels.size(); ++i)
			{
				Fl_Color fontColor = fl_rgb_color(model->PieChartUnit.Channels[i].FontColor.R, model->PieChartUnit.Channels[i].FontColor.G, model->PieChartUnit.Channels[i].FontColor.B);
				//fl_color(fontColor);
				GraphicDrawHandle::Ins()->SetBrushStyle(fontColor, 0, 0, model->PieChartUnit.Alpha);
				fl_draw(model->PieChartUnit.Channels[i].text.c_str(), TxtPosList[i].first,TxtPosList[i].second);
			}
		}
	}

	void PieChartView::DrawPie(Project::PieChartChannelStyle& style, Rectangle& rect, int dir, Project::StringStyle& textStyle) {
		shared_ptr<PieChartModel> model = BaseView.GetModel<PieChartModel>();
		// ��ȡ��������
		Fl_Color backColor = fl_rgb_color(style.BackColor.R, style.BackColor.G, style.BackColor.B);
		//fl_color(backColor);
		GraphicDrawHandle::Ins()->SetBrushStyle(backColor, 0, 0, model->PieChartUnit.Alpha);
		// �������ֽǶ�
		int textAngle = 0;
		// ���Ʊ�ͼ����
		if (dir == 0) {
			fl_pie(rect.X, rect.Y, rect.W, rect.H, style.EndAngle, style.StartAngle);
			if (style.StartAngle > style.EndAngle)
				textAngle = style.EndAngle + (style.StartAngle - style.EndAngle) / 2;
			else
				textAngle = style.EndAngle + (360 + style.StartAngle - style.EndAngle) / 2;
			if (textAngle > 360)
				textAngle -= 360;
		}
		else {
			fl_pie(rect.X, rect.Y, rect.W, rect.H, style.StartAngle, style.EndAngle);
			textAngle = style.StartAngle + (style.EndAngle - style.StartAngle) / 2;
		}
		// ��ȡ��������
		int fontSize = textStyle.Font.Size;
		int fontStyle = UI::IResourceService::GetFontIdx(textStyle.Font.Name);
		
		// ��ȡ������ɫ
		/*Fl_Color fontColor = fl_rgb_color(style.FontColor.R, style.FontColor.G, style.FontColor.B);
		fl_color(fontColor);*/
		// ��������
		double textX = cos(textAngle * M_PI / 180) * (rect.W / 4 + 10);
		double textY = sin(textAngle * M_PI / 180) * (rect.H / 4 + 10);
		// ���ֳߴ�
		int textW = fl_width(style.text.c_str(), strlen(style.text.c_str()));
		int textH = fl_height();
		// ������������
		//fl_draw(style.text.c_str(),rect.X + rect.W / 2 + (int)textX - textW / 2, rect.Y + rect.H / 2 - (int)textY + textH / 2);
		TxtPosList.push_back(std::make_pair( rect.X + rect.W / 2 + (int)textX - textW / 2, rect.Y + rect.H / 2 - (int)textY + textH / 2 ));
	}
}
