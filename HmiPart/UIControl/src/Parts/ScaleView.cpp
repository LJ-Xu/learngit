#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "UIComm.h"
#include "ScaleView.h"
#include "ScaleControl.h"
#include "ScaleModel.h"
#include "StringUtility.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Output.H>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "UIData.h"
#include "AdvancedGraphic.h"


namespace UI
{
	void ScaleView::setLineStyle(ScaleModel* model)
	{
		fl_color(RGBColor(model->ScaleConfig.LineColor));
		fl_line_style(model->ScaleConfig.LineStyle, model->ScaleConfig.LineWidth);
	}
	//根据角度和半径计算当前圆上的点相对于中心点的偏移位置（参考3点为0度，逆时针）
	//向右和向下偏移为正，向左和向上为负
	void ScaleView::calculatePosOnCircle(double angle, int radius, int& offsetX, int& offsetY)
	{
		if (angle > 360)
		{
			angle = angle - 360;
		}
		double arc = angle * M_PI / 180;
		if (0 <= angle && 90 > angle)
		{
			offsetX = cos(arc)*radius;
			offsetY = -sin(arc)*radius;
		}
		else if (90 <= angle && 180 > angle)
		{
			offsetX = -cos(M_PI - arc)*radius;
			offsetY = -sin(M_PI - arc)*radius;
		}
		else if (180 <= angle && 270 > angle)
		{
			offsetX = -sin(M_PI * 3 / 2 - arc)*radius;
			offsetY = cos(M_PI * 3 / 2 - arc)*radius;
		}
		else
		{
			offsetX = cos(M_PI * 2 - arc)*radius;
			offsetY = sin(M_PI * 2 - arc)*radius;
		}
	}


	ScaleView::ScaleView(int X, int Y, int W, int H):HMIBaseView(X, Y, W, H)
	{
		CurMinValue = 0;
		CurMaxValue = 100;
		memset(GradFormat, 0, 20);
	}

	ScaleView::~ScaleView()
	{
	}



	void ScaleView::draw()
	{
		shared_ptr<ScaleControl> ctrl = BaseView.GetControl<ScaleControl>();
		shared_ptr<ScaleModel> model = BaseView.GetModel<ScaleModel>();
		FinX = model->ScaleConfig.X + model->ScaleConfig.OffX;
		FinY = model->ScaleConfig.Y + model->ScaleConfig.OffY;
		FinW = model->ScaleConfig.Width;
		FinH = model->ScaleConfig.Height;
		GraphicDrawHandle::PushClip(FinX, FinY, FinW, FinH);
		/*FinX += model->ScaleConfig.MarkFontSytle.Font.Size;
		FinY += model->ScaleConfig.MarkFontSytle.Font.Size;
		FinW -= model->ScaleConfig.MarkFontSytle.Font.Size * 4;
		FinH -= model->ScaleConfig.MarkFontSytle.Font.Size * 4;*/
		DrawStickChart(ctrl.get(), model.get());
		fl_pop_clip();
	}

	void ScaleView::DrawStickChart(ScaleControl * ctrl, ScaleModel * model)
	{
		//calculateCurLimit(model);
		initFormat(model);
		switch (model->ScaleConfig.ScaleTp)
		{
		case model->ScaleConfig.Vertical:
		case model->ScaleConfig.Horizontal:
			drawonstick(model);
			break;
		case model->ScaleConfig.UpperSemicircle:
		case model->ScaleConfig.LowerSemicircle:
		case model->ScaleConfig.Circular:
		case model->ScaleConfig.UDCir:
			drawonarc(model);
			break;
		}
	}

	void ScaleView::initFormat(ScaleModel* model)
	{
		if (GradFormat[0] != 0)
		{
			return;
		}
		/*int mainLen = (model->ScaleConfig.MarkDecimalNumber > 0 ? model->ScaleConfig.MarkDecimalNumber + 1 : 0) + model->ScaleConfig.MarkIntegerNumber;
		if (mainLen > 0)
		{
			snprintf(GradFormat, 20, "%%0%d.%df", 1, model->ScaleConfig.MarkDecimalNumber);
		}*/
		snprintf(GradFormat, 20, "%%0%d.%df", 1, model->ScaleConfig.MarkDecimalNumber);
		fontWidth = model->ScaleConfig.MarkFontSytle.Font.Size / 4 * (model->ScaleConfig.MarkIntegerNumber + model->ScaleConfig.MarkDecimalNumber);

	}
	//画在直条上
	void ScaleView::drawonstick(ScaleModel * model)
	{
		if (model->ScaleConfig.MainGraduateCount > 1)
		{
			int sx = FinX, sy = FinY;
			bool ishor = false;
			int len = 0;
			

			switch (model->ScaleConfig.ScaleTp)
			{
			case model->ScaleConfig.Vertical:
				len = FinH;
				if (model->ScaleConfig.MarkPlace==1)
					sx = FinX + FinW - model->ScaleConfig.MajorScaleLineLength;
				break;
			case model->ScaleConfig.Horizontal:
				len = FinW;
				ishor = true;
				if (model->ScaleConfig.MarkPlace==1)
					sy = FinY + FinH - model->ScaleConfig.MajorScaleLineLength;
				break;
			default:
				return;
			}
			int mx = sx, my = sy;
			int marksz = model->ScaleConfig.MarkFontSytle.Font.Size*(model->ScaleConfig.MarkIntegerNumber + model->ScaleConfig.MarkDecimalNumber+1) / 2;
			//给刻度数字留空间
			if (model->ScaleConfig.IsDisplayMark)
			{
				switch (model->ScaleConfig.ScaleTp)
				{
				case model->ScaleConfig.Vertical:
					if (model->ScaleConfig.MarkPlace==1)
					{
						sx = FinX + FinW - model->ScaleConfig.MajorScaleLineLength;
						mx = sx - marksz;
						//绘制轴线
						if (model->ScaleConfig.IsDisplayAxis)
						{
							fl_color(RGBColor(model->ScaleConfig.LineColor));
							fl_line_style(model->ScaleConfig.LineStyle, model->ScaleConfig.LineWidth);
							fl_line(FinX + FinW,
								sy,
								FinX + FinW,
								sy + len);
						}

					}
					else
					{
						mx = sx + model->ScaleConfig.MajorScaleLineLength;
						//绘制轴线
						if(model->ScaleConfig.IsDisplayAxis)
						{
							fl_color(RGBColor(model->ScaleConfig.LineColor));
							fl_line_style(model->ScaleConfig.LineStyle, model->ScaleConfig.LineWidth);
							fl_line(sx,
								sy,
								sx,
								sy + len);
						}
					}
					break;
				case model->ScaleConfig.Horizontal:
					if (model->ScaleConfig.MarkPlace == 1)
					{
						my = sy - model->ScaleConfig.MarkFontSytle.Font.Size;
						//绘制轴线
						if(model->ScaleConfig.IsDisplayAxis)
						{
							fl_color(RGBColor(model->ScaleConfig.LineColor));
							fl_line_style(model->ScaleConfig.LineStyle, model->ScaleConfig.LineWidth);
							fl_line(sx,
								FinY + FinH,
								sx + len,
								FinY + FinH);
						}
					}
					else
					{
						my = sy + model->ScaleConfig.MajorScaleLineLength+ model->ScaleConfig.MarkFontSytle.Font.Size;
						//绘制轴线
						if (model->ScaleConfig.IsDisplayAxis)
						{
							fl_color(RGBColor(model->ScaleConfig.LineColor));
							fl_line_style(model->ScaleConfig.LineStyle, model->ScaleConfig.LineWidth);
							fl_line(sx,
								sy,
								sx + len,
								sy);
						}
					}
					break;
				default:
					return;
				}
			}
			
			AdvancedGarphic::DrawScale(ishor, (model->ScaleConfig.MarkPlace+1)%2, sx, sy, len, model->ScaleConfig.MainGraduateCount, model->ScaleConfig.UseSecondaryScale?model->ScaleConfig.SecondaryScaleCutCount:0, model->ScaleConfig.MajorScaleLineLength, model->ScaleConfig.SecondaryScaleLength, model->ScaleConfig.LineStyle, model->ScaleConfig.LineColor, model->ScaleConfig.LineWidth);
			//数字标记
			if ((model->ScaleConfig.IsDisplayMark) && (model->ScaleConfig.MainGraduateCount > 1) && GradFormat[0] != '0')
			{
				int dir = 0;
				switch (model->ScaleConfig.ScaleTp)
				{
				case 0://水平
					if (model->ScaleConfig.IsReserveMark)
						dir = 2;
					else
						dir = 3;
					break;
				case 1:
					if (model->ScaleConfig.IsReserveMark)
						dir = 1;
					else
						dir = 0;
					break;
				}
				AdvancedGarphic::DrawScaleMark(((model->ScaleConfig.ScaleTp+1 )%2)*2 + (model->ScaleConfig.IsReserveMark ? 1 : 0), mx, my, len, model->ScaleConfig.MarkFontSytle, model->ScaleConfig.MainGraduateCount, (float)(CurMinValue), (float)(CurMaxValue), GradFormat,model->ScaleConfig.MarkIntegerNumber, active());
			}
		}
	}
	//画在圆形上
	void ScaleView::drawonarc(ScaleModel * model)
	{
		int curRadius = 0;//半径
		int centerX, centerY;//中心点坐标
		double realStartAngle, realEndAngle,realSweepAngle;//针对fltk修正后的起始和终点角度
		int leaveSizeForFont = (model->ScaleConfig.IsDisplayMark&&(model->ScaleConfig.MarkPlace==1)) ? fontWidth*2 : 0;
		switch (model->ScaleConfig.ScaleTp)
		{
		case model->ScaleConfig.UpperSemicircle:
			curRadius = (FinW / 2 >= FinH ? FinH : FinW / 2)- leaveSizeForFont;
			realStartAngle = 0.0f;
			realEndAngle = 180.0f;
			realSweepAngle = 180.0f;
			centerX = FinX + FinW/2;
			centerY = FinY + FinH;
			break;
		case model->ScaleConfig.LowerSemicircle:
			curRadius = (FinW / 2 >= FinH ? FinH : FinW / 2)- leaveSizeForFont;
			realStartAngle = 180.0f;
			realEndAngle = 360.0f;
			realSweepAngle = 180.0f;
			centerX = FinX + FinW / 2;
			centerY = FinY;
			break;
		case model->ScaleConfig.Circular:
			curRadius = (FinW <
				FinH ? FinW / 2 : FinH / 2)- leaveSizeForFont;
			realStartAngle = 0.0f;
			realEndAngle = 360.0f;
			realSweepAngle = 360.0f;
			centerX = FinX + FinW / 2;
			centerY = FinY + FinH / 2;
			break;
		case model->ScaleConfig.UDCir:
			curRadius = (FinW / 2 >= FinH/2 ? FinH/2 : FinW / 2) - leaveSizeForFont;
			realStartAngle = model->ScaleConfig.CircularStartAngle;
			realEndAngle = model->ScaleConfig.CircularEndAngle;
			realSweepAngle = model->ScaleConfig.CircularEndAngle- model->ScaleConfig.CircularStartAngle;
			centerX = FinX + FinW / 2;
			centerY = FinY + FinH/2;
			break;
		default:
			return;
		}
		if (model->ScaleConfig.IsDisplayAxis)
		{
			fl_color(RGBColor(model->ScaleConfig.LineColor));
			fl_line_style(model->ScaleConfig.LineStyle, model->ScaleConfig.LineWidth);
			fl_arc(centerX- curRadius,
				centerY- curRadius,
				curRadius * 2,
				curRadius * 2, realStartAngle, realEndAngle);
		}
		//主刻度和副刻度各自的角度间隔（角度）
		double mOffset = 0;
		double sOffset = 0;
		bool bSec = model->ScaleConfig.SecondaryScaleCutCount > 0 && model->ScaleConfig.UseSecondaryScale;
		if (model->ScaleConfig.MainGraduateCount > 1)
		{
			mOffset = (float)realSweepAngle / (float)(model->ScaleConfig.MainGraduateCount - 1);
			if (bSec)
			{
				sOffset = (float)mOffset / (float)(model->ScaleConfig.SecondaryScaleCutCount + 1);
			}
			//角分坐标
			double curLineAngle = realStartAngle;
			//当前坐标
			int lineStartX, lineStartY;
			int lineEndX, lineEndY;
			//线段收尾坐标的相对圆心的半径长度，根据内环和外环时有不同的延生方向
			int curMainLineStartPtRadius=0, curSecLineStartPtRadius=0;
			int curMainLineEndPtRadius=0, curSecLineEndPtRadius=0;
			//数字标记坐标的中心点相对圆心的半径长度，区分情况同上
			//int curGraduatePtRadius;
			switch (model->ScaleConfig.MarkPlace)
			{
			case 1://外,相对圆心从外向内画
				curMainLineStartPtRadius = curRadius;
				curMainLineEndPtRadius = curMainLineStartPtRadius - model->ScaleConfig.MajorScaleLineLength;

				if (bSec)
				{
					curSecLineStartPtRadius = curRadius;
					curSecLineEndPtRadius = curSecLineStartPtRadius - model->ScaleConfig.SecondaryScaleLength;
				}
				break;
			case 0://内/居中（不用），相对圆心从外向内画
			case 2:
				
				curMainLineStartPtRadius = curRadius;
				curMainLineEndPtRadius = curMainLineStartPtRadius - model->ScaleConfig.MajorScaleLineLength;
				if (bSec)
				{
					curSecLineStartPtRadius = curRadius;
					curSecLineEndPtRadius = curSecLineStartPtRadius - model->ScaleConfig.SecondaryScaleLength;
				}
				break;
			}

			int curSAngle = 0;
			//线段样式
			setLineStyle(model);
			//画线
			for (int idxM = 0; idxM < model->ScaleConfig.MainGraduateCount; ++idxM)
			{
				calculatePosOnCircle(curLineAngle, curMainLineStartPtRadius, lineStartX, lineStartY);
				calculatePosOnCircle(curLineAngle, curMainLineEndPtRadius, lineEndX, lineEndY);
				fl_line(centerX + lineStartX, centerY + lineStartY, centerX + lineEndX, centerY + lineEndY);

				if (model->ScaleConfig.UseSecondaryScale&&(idxM < model->ScaleConfig.MainGraduateCount - 1))
				{
					for (int idxS = 1; idxS <= model->ScaleConfig.SecondaryScaleCutCount; ++idxS)
					{
						curSAngle = curLineAngle + idxS * sOffset;
						calculatePosOnCircle(curSAngle, curRadius, lineStartX, lineStartY);
						calculatePosOnCircle(curSAngle, curSecLineEndPtRadius, lineEndX, lineEndY);
						fl_line(centerX + lineStartX, centerY + lineStartY, centerX + lineEndX, centerY + lineEndY);
					}
				}
				curLineAngle += mOffset;
			}
			if (model->ScaleConfig.IsDisplayMark)
			{
				double startAngle=0.0, sweepAngle=360.0;
				int cx, cy;
				switch (model->ScaleConfig.ScaleTp)
				{
				case ScaleConfig::ScaleType::UpperSemicircle:
					cx = FinX + FinW / 2;
					cy = FinY + FinH;
					if (!model->ScaleConfig.IsReserveMark)
					{
						startAngle = 0.0;
						sweepAngle = 180.0;
					}
					else
					{
						startAngle = 180.0;
						sweepAngle = -180.0;
					}
					break;
				case ScaleConfig::ScaleType::LowerSemicircle:
					cx = FinX + FinW / 2;
					cy = FinY;
					if (!model->ScaleConfig.IsReserveMark)
					{
						startAngle = 180.0;
						sweepAngle = 180.0;
					}
					else
					{
						startAngle = 360;
						sweepAngle = -180.0;
					}
					break;
				case ScaleConfig::ScaleType::Circular:
					cx = FinX + FinW / 2;
					cy = FinY + FinH / 2;
					if (!model->ScaleConfig.IsReserveMark)
					{
						startAngle = 0.0;
						sweepAngle = 360.0;
					}
					else
					{
						startAngle = 0;
						sweepAngle = -360.0;
					}
					break;
				case model->ScaleConfig.UDCir:
					cx = FinX + FinW / 2;
					cy = FinY + FinH / 2;
					if (!model->ScaleConfig.IsReserveMark)
					{
						startAngle = realStartAngle;
						sweepAngle = realSweepAngle;
					}
					else
					{
						startAngle = realStartAngle + realSweepAngle;
						sweepAngle = -realSweepAngle;
					}
					break;
				}
				int markRd = (model->ScaleConfig.MarkPlace != 1)?(curMainLineStartPtRadius - model ->ScaleConfig.MarkFontSytle.Font.Size- model->ScaleConfig.MajorScaleLineLength):( curMainLineStartPtRadius + model->ScaleConfig.MarkFontSytle.Font.Size);
				AdvancedGarphic::DrawScaleMark(cx, cy,model->ScaleConfig.MarkFontSytle,  markRd, model->ScaleConfig.MainGraduateCount,(float)this->CurMinValue, (float)this->CurMaxValue, startAngle, sweepAngle, GradFormat, model->ScaleConfig.MarkIntegerNumber, false,active());

				}

			}

		

	}
}

