#include "stdafx.h"
#include "UIComm.h"
#include "DashboardView.h"
#include "DashboardControl.h"
#include "DashboardModel.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Output.H>
//#include<gl/glut.h>
#include "AdvancedGraphic.h"
#include "RotatePic.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "UIData.h"
#define KEEPWHOLECIRCLE 1
#define Default_Point_Angle 180
#define Default_Start_Angle 90
namespace UI
{
	DashboardView::DashboardView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H), DashBoardImgPtr(nullptr), TempWatchHandleImgPtr(nullptr), TempAxisImgPtr(nullptr), WatchHandleBuffer(nullptr), AxisBuffer(nullptr), HmiMainMode(false),FD(0.0), GradFormat()
	{
		//memset(GradFormat, 0, 20);
	}


	DashboardView::~DashboardView()
	{
		if (DashBoardImgPtr != nullptr)
		{
			Fl_RGB_Image *rgbimage = (Fl_RGB_Image *)DashBoardImgPtr;
			const uchar *data = rgbimage->array;
			delete DashBoardImgPtr;
			delete[] data;
			DashBoardImgPtr = nullptr;
		}
		//if (TempWatchHandleImgPtr != nullptr)
		//{
		//	//Fl_RGB_Image *rgbimage = (Fl_RGB_Image *)TempWatchHandleImgPtr;
		//	//const uchar *data = rgbimage->array;
		//	delete TempWatchHandleImgPtr;
		//	//delete[] data;
		//	TempWatchHandleImgPtr = nullptr;
		//}
		//if (TempAxisImgPtr != nullptr)
		//{
		//	//Fl_RGB_Image *rgbimage = (Fl_RGB_Image *)TempAxisImgPtr;
		//	//const uchar *data = rgbimage->array;
		//	delete TempAxisImgPtr;
		//	//delete[] data;
		//	TempAxisImgPtr = nullptr;
		//}
	}

	//刻度数字表达式
	void DashboardView::initformat(DashboardModel* model)
	{
		if (GradFormat[0] != 0)
		{
			return;
		}
		int mainLen = (model->DashboardConfig.DecimalDigit > 0 ? model->DashboardConfig.DecimalDigit + 1 : 0) + model->DashboardConfig.IntegerDigit;
		switch (model->DashboardConfig.ShowGraduateSign)
		{
		case 0:
			return;
		case 1:
			if (mainLen < 1)
				return;
			snprintf(GradFormat, 20, "%%0%d.%df", 1, model->DashboardConfig.DecimalDigit);
			break;
		case 2:
			snprintf(GradFormat, 20, "%%0%d.%df%%%%", 1, model->DashboardConfig.DecimalDigit);
			break;
		default:
			break;
		} 
	}
	//绘制表盘（背景与危险色）
	void DashboardView::drawboard(DashboardModel* model)
	{
		//背景
		if (DashBoardImgPtr!=nullptr) {

			image(DashBoardImgPtr);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
			box(FL_NO_BOX);
			draw_box();
		}
		else
		{
			Fl_Image* tempimg = IResourceService::Ins()->GetImage(model->DashboardConfig.DashBoardStyle.KeyVal);
			if (tempimg)
			{
				image(tempimg);
				align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
				box(FL_NO_BOX);
				draw_box();
			}
			
		}
		//常规填充处
		/*fl_color(RGBColor(model->DashboardConfig.DataColor));
		fl_pie(FinX,
			FinY,
			model->DashboardConfig.Width, model->DashboardConfig.Width,
			ArcStartAngle, ArcEndAngle);*/
		if (!MaxValueValid||FD==0)
			return;
		const int wholeRadius = model->DashboardConfig.Width / 2;
		const int centerX = FinX + wholeRadius;
		const int centerY = FinY + wholeRadius;
		int modrd = model->DashboardConfig.RingToCircleCenterLength + model->DashboardConfig.NormalRingWidth / 2;
		double normalstartA= ArcStartAngle, normalendA= ArcEndAngle;
		//危险色区
		if (UpperDangerValueValid)
		{
			double dangerLowerStartAngle, dangerLowerEndAngle, dangerUpperStartAngle, dangerUpperEndAngle;
			if (!model->DashboardConfig.ArcDirection)
			{
				dangerLowerEndAngle = normalendA;
				//dangerLowerEndAngle = ArcEndAngle;
				dangerLowerStartAngle = ArcEndAngle + ((double)FD * CurrentLowerDangerValue);
				dangerUpperStartAngle = normalstartA;
				//dangerUpperStartAngle = ArcStartAngle;
				dangerUpperEndAngle = ArcEndAngle + ((double)FD * CurrentUpperDangerValue);
				normalendA = dangerLowerStartAngle;
				normalstartA = dangerUpperEndAngle;
			}
			else
			{
				dangerLowerStartAngle = normalstartA;
				//dangerLowerStartAngle = ArcStartAngle;
				dangerLowerEndAngle = ArcStartAngle + ((double)FD * CurrentLowerDangerValue);
				dangerUpperEndAngle = normalendA;
				//dangerUpperEndAngle = ArcEndAngle;
				dangerUpperStartAngle = ArcStartAngle + ((double)FD * CurrentUpperDangerValue);
				normalendA = dangerUpperStartAngle;
				normalstartA = dangerLowerEndAngle;
			}
			drawsector(FinX + model->DashboardConfig.Width / 2, FinY + model->DashboardConfig.Height / 2, modrd, model->DashboardConfig.NormalRingWidth, dangerLowerStartAngle, dangerLowerEndAngle, model->DashboardConfig.DangerColor);
			drawsector(FinX + model->DashboardConfig.Width / 2, FinY + model->DashboardConfig.Height / 2, modrd, model->DashboardConfig.NormalRingWidth, dangerUpperStartAngle, dangerUpperEndAngle, model->DashboardConfig.DangerColor);

		}
		//报警色区
		if (UpperWarnValueValid)
		{
			double upperStartAngle, upperEndAngle, lowerStartAngle, lowerEndAngle;
			//顺时针则反转
			if (!model->DashboardConfig.ArcDirection)
			{
				upperStartAngle = normalstartA;
				upperEndAngle = ArcEndAngle + (CurrentUpperWarnValue * (double)FD);
				lowerEndAngle = normalendA;
				lowerStartAngle = ArcEndAngle + (CurrentLowerWarnValue * (double)FD);
				normalstartA = upperEndAngle;
				normalendA = lowerStartAngle;
				/*ArrNormalFieldAngleGroup[3] = lowerStartAngle;
				ArrNormalFieldAngleGroup[0] = upperEndAngle;*/
			}
			else
			{
				upperEndAngle = normalendA;
				upperStartAngle = ArcStartAngle + (CurrentUpperWarnValue * (double)FD);
				lowerStartAngle = normalstartA;
				lowerEndAngle = ArcStartAngle + (CurrentLowerWarnValue * (double)FD);
				normalstartA = lowerEndAngle;
				normalendA = upperStartAngle;
				/*ArrNormalFieldAngleGroup[0] = lowerEndAngle;
				ArrNormalFieldAngleGroup[3] = upperStartAngle;*/
			}

			drawsector(FinX + model->DashboardConfig.Width / 2, FinY + model->DashboardConfig.Height / 2, modrd, model->DashboardConfig.NormalRingWidth, upperStartAngle, upperEndAngle, model->DashboardConfig.UpperWarnColor);
			drawsector(FinX + model->DashboardConfig.Width / 2, FinY + model->DashboardConfig.Height / 2, modrd, model->DashboardConfig.NormalRingWidth, lowerStartAngle, lowerEndAngle, model->DashboardConfig.LowerWarnColor);

		}

		drawsector(FinX + model->DashboardConfig.Width / 2, FinY + model->DashboardConfig.Height / 2, modrd, model->DashboardConfig.NormalRingWidth, normalstartA, normalendA, model->DashboardConfig.DataColor);
	}
	//绘制表针与轴心
	void DashboardView::drawwathhandle(DashboardModel* model)
	{
		//double currentAngle = model->DashboardConfig.ArcDirection ? (ArcStartAngle + (double)FD * CurrentValue) : (ArcEndAngle-(double)FD * CurrentValue);
		double currentAngle = model->DashboardConfig.ArcDirection?(ArcStartAngle + (double)FD * CurrentValue): (ArcEndAngle + (double)FD * CurrentValue);
		//表针与轴心
		drawrotateimage(model, currentAngle);
	}
	//显示刻度
	void DashboardView::drawgraduate(DashboardModel* model)
	{
		if (!model->DashboardConfig.ShowGraduate)
			return;
		int curRadius = (int)((float)(model->DashboardConfig.Width< model->DashboardConfig.Height ?model->DashboardConfig.Width / 2: model->DashboardConfig.Height/2)*12.0f/20.0f);//半径
		//int curRadius = (model->DashboardConfig.GraduatePosition!=1)?  model->DashboardConfig.RingToCircleCenterLength: (model->DashboardConfig.RingToCircleCenterLength + model->DashboardConfig.NormalRingWidth );
		int centerX= FinX + model->DashboardConfig.Width / 2, centerY= FinY+ model->DashboardConfig.Height / 2;//中心点坐标
		//double realSweepAngle= model->DashboardConfig.ArcDirection?ArcEndAngle - ArcStartAngle: ArcStartAngle- ArcEndAngle;//经过角度
		double realSweepAngle = ArcEndAngle - ArcStartAngle;
		fl_color(RGBColor(model->DashboardConfig.GraduateColor));
		fl_line_style(0, 3);
		fl_arc(centerX- curRadius,
			centerY- curRadius,
			curRadius * 2,
			curRadius * 2, ArcStartAngle, ArcEndAngle);
		//主刻度和副刻度各自的角度间隔（角度）
		double mOffset = 0;
		double sOffset = 0;
		bool bSec = model->DashboardConfig.SecondaryGraduateCount > 0;
		if (model->DashboardConfig.MainGraduateCount > 1)
		{
			mOffset = realSweepAngle / (model->DashboardConfig.MainGraduateCount - 1.0f);
			if (bSec)
			{
				sOffset = mOffset / (model->DashboardConfig.SecondaryGraduateCount + 1.0f);
			}
			//角分坐标
			double curLineAngle = ArcStartAngle;
			//当前坐标
			int lineStartX, lineStartY;
			int lineEndX, lineEndY;
			//int fontCX, fontCY;
			//线段收尾坐标的相对圆心的半径长度，根据内环和外环时有不同的延生方向
			int curMainLineStartPtRadius, curSecLineStartPtRadius;
			int curMainLineEndPtRadius, curSecLineEndPtRadius;
			//数字标记坐标的中心点相对圆心的半径长度，区分情况同上
			int curGraduatePtRadius;
			switch (model->DashboardConfig.GraduatePosition)
			{
			case 1://外
				curMainLineStartPtRadius = curRadius;
				curMainLineEndPtRadius = curRadius + model->DashboardConfig.MainGraduateCount;
				if (bSec)
				{
					curSecLineStartPtRadius = curRadius;
					curSecLineEndPtRadius = curRadius + model->DashboardConfig.SecondaryGraduateLen;
				}
				if (model->DashboardConfig.ShowGraduateSign)
				{
					curGraduatePtRadius = curMainLineEndPtRadius + model->DashboardConfig.TypeFaceGraduate.Font.Size / 2;
				}
				break;
			case 0://内
				curMainLineStartPtRadius = curRadius;
				curMainLineEndPtRadius = curRadius - model->DashboardConfig.MainGraduateLen;

				if (bSec)
				{
					curSecLineStartPtRadius = curRadius;
					curSecLineEndPtRadius = curRadius - model->DashboardConfig.SecondaryGraduateLen;
				}
				if (model->DashboardConfig.ShowGraduateSign)
				{
					curGraduatePtRadius = curMainLineEndPtRadius - model->DashboardConfig.TypeFaceGraduate.Font.Size / 2;
				}
				break;
			case 2://居中
				curMainLineStartPtRadius = curRadius - model->DashboardConfig.MainGraduateLen / 2;
				curMainLineEndPtRadius = curRadius + model->DashboardConfig.MainGraduateLen / 2;
				if (bSec)
				{
					curSecLineStartPtRadius = curRadius - model->DashboardConfig.SecondaryGraduateLen;
					curSecLineEndPtRadius = curRadius + model->DashboardConfig.SecondaryGraduateLen;
				}
				if (model->DashboardConfig.ShowGraduateSign)
				{
					curGraduatePtRadius = curMainLineEndPtRadius + model->DashboardConfig.TypeFaceGraduate.Font.Size / 2;
				}
				break;
			}

			int curSAngle = 0;
			//线段样式
			setlinestyle(model);
			//画线
			for (int idxM = 0; idxM < model->DashboardConfig.MainGraduateCount; ++idxM)
			{
				calculatepospncircle(curLineAngle, curMainLineStartPtRadius, lineStartX, lineStartY);
				calculatepospncircle(curLineAngle, curMainLineEndPtRadius, lineEndX, lineEndY);
				fl_line_style(0, 3);
				fl_line(centerX + lineStartX, centerY + lineStartY, centerX + lineEndX, centerY + lineEndY);

				fl_line_style(0, 1);
				if (idxM < model->DashboardConfig.MainGraduateCount - 1)
				{
					for (int idxS = 1; idxS <= model->DashboardConfig.SecondaryGraduateCount; ++idxS)
					{
						curSAngle = curLineAngle + idxS * sOffset;
						calculatepospncircle(curSAngle, curRadius, lineStartX, lineStartY);
						calculatepospncircle(curSAngle, curSecLineEndPtRadius, lineEndX, lineEndY);
						fl_line(centerX + lineStartX, centerY + lineStartY, centerX + lineEndX, centerY + lineEndY);
					}
				}
				curLineAngle += mOffset;
			}
			
			if (model->DashboardConfig.ShowGraduateSign)
			{
				double startAngle = model->DashboardConfig.ArcDirection ? ArcStartAngle: ArcEndAngle, sweepAngle = model->DashboardConfig.ArcDirection? ArcEndAngle - ArcStartAngle : ArcStartAngle - ArcEndAngle;
				//int cx= centerX, cy;

				AdvancedGarphic::DrawScaleMark(centerX, centerY, model->DashboardConfig.TypeFaceGraduate, curGraduatePtRadius, model->DashboardConfig.MainGraduateCount, (float)this->CurrentMinValue, (float)this->CurrentMaxValue, startAngle, sweepAngle, GradFormat, model->DashboardConfig.IntegerDigit,model->DashboardConfig.ShowGraduateSign==2,active());

			}
		}
	}

	void DashboardView::drawsector(int cx, int cy,int r, int w, double sA, double eA,int color)
	{
		//计算绘制尺寸
		//int sz = r + w;
		int sz = r;
		//计算绘制起点
		int ix = cx - (sz), iy = cy -(sz);
		//Fl_Color linecolor = fl_rgb_color(RGBColor(color));
		//fl_color(active() ? linecolor : fl_inactive(linecolor));
		fl_color(fl_rgb_color(RGBColor(color)));
		fl_line_style(FL_CAP_SQUARE | FL_JOIN_MITER, w);
		fl_begin_line();
		int startx, starty, endx, endy;
		startx = cx +sz * cos(sA*PI/180.0);
		starty = cy -sz * sin(sA*PI / 180.0);
		endx = cx +sz * cos(eA*PI / 180.0);
		endy = cy -sz * sin(eA*PI / 180.0);
#if 1
#ifdef WIN32
		if (fabs(sA - eA) < 90) {
			if (startx == endx && starty == endy) SetPixel(fl_gc, startx, starty, fl_RGB());
			else Arc(fl_gc, ix, iy, ix + sz * 2, iy + sz * 2, startx, starty, endx, endy);
		}
		else 
			Arc(fl_gc, ix, iy, ix+sz*2, iy + sz * 2, startx, starty, endx, endy);
#else //X11
		/*fl_arc(arcX, arcY, width * 2, height * 2, model->ArcConfig.S_Angle, model->ArcConfig.E_Angle);*/
		fl_arc(ix, iy, r, sA, eA);
#endif
#else
		for (int i = 0; i < w; ++i)
		{
			fl_arc(ix++, iy++, sz*2, sz*2, sA, eA);
			//--sz;
		}
#endif
		fl_end_line();
	}

	//加载图片
	void DashboardView::loadimages(DashboardModel* model)
	{
		//背景图
		if (DashBoardImgPtr == nullptr)
		{
			Fl_Image* tempimg = nullptr;
			if (model->DashboardConfig.IsCuttedBoarder)
			{
				tempimg = IResourceService::Ins()->GetImage(model->DashboardConfig.CuttedBoardStyle.KeyVal);
			}
			else
			{
				tempimg = IResourceService::Ins()->GetImage(model->DashboardConfig.DashBoardStyle.KeyVal);
			}
			

			if (tempimg)
			{
				int imgPreDataLen = tempimg->w()*tempimg->h();
				int imtAftDataLen = imgPreDataLen * 4;
				int fadeValue = (int)(255.0 * (float)model->DashboardConfig.Transparency / 100.0);
				//透明化
				if (tempimg->d() != 4)
				{
					uchar *rgbFadeBefore = new uchar[imtAftDataLen];
					uchar *rgbFadeAfter = new uchar[imgPreDataLen * 4];
					memcpy(rgbFadeBefore, tempimg->data()[0], imtAftDataLen);
					for (int i = 0, j = 0; j < imtAftDataLen; i += 3, j += 4)
					{
						rgbFadeAfter[j] = rgbFadeBefore[i];
						rgbFadeAfter[j + 1] = rgbFadeBefore[i + 1];
						rgbFadeAfter[j + 2] = rgbFadeBefore[i + 2];
						rgbFadeAfter[j + 3] = fadeValue;
					}
					DashBoardImgPtr = new Fl_RGB_Image(rgbFadeAfter, tempimg->w(), tempimg->h(), 4);
					delete[] rgbFadeBefore;
				}
				else
				{
					uchar *rgba = new uchar[imgPreDataLen * 4];
					memcpy(rgba, tempimg->data()[0], imgPreDataLen * 4);
					/*for (int i = 0; i < imtAftDataLen; i += 4)
					{
						rgba[i + 3] = fadeValue;
					}*/
					DashBoardImgPtr = new Fl_RGB_Image(rgba, tempimg->w(), tempimg->h(), 4);
				}
			}
		}
		//表针图
		TempWatchHandleImgPtr = IResourceService::Ins()->GetImage(model->DashboardConfig.WatchHandleStyle.KeyVal);
		//轴心图
		TempAxisImgPtr = IResourceService::Ins()->GetImage(model->DashboardConfig.AxisStyle.KeyVal);
	}

	void DashboardView::drawrotateimage(DashboardModel* model,double angle)
	{
		if (!TempWatchHandleImgPtr)
			return;
		double arc = (PI * 2) * (0 * 1.0 / 1.0 + angle / 360.0);
		//指针实际转动角度
		double whAngle = angle - Default_Point_Angle;
		double whArc = (PI * 2) * (0 * 1.0 / 1.0 + whAngle / 360.0);
		//获取源图片数据
		
		//表针
		{
			//公转半径,同时也是变换后的指针图像的矩形范围
			int R = (int)(::sqrt(1.0 * TempWatchHandleImgPtr->w() * TempWatchHandleImgPtr->w() + TempWatchHandleImgPtr->h() * TempWatchHandleImgPtr->h()) +4+ 0.5);
			//long R = TempWatchHandleImgPtr->w();
			//自转(变换后的指针矩形区所在圆)半径
			int r = R * 0.5;
			//指针旋转中心(rx0,ry0),以整张仪表盘的左上角为(0,0)
			int rx0 = model->DashboardConfig.Width*0.5;
			int ry0 = model->DashboardConfig.Height*0.5;
			//自转中心
			double irx0 = TempWatchHandleImgPtr->w() * 0.5;
			double iry0 = TempWatchHandleImgPtr->h() * 0.5;
			//原图像在数学计算时，重置入圆形轨迹所在的正方形矩形区域后的像素偏移量（如正方形原点(0,0)，则原指针的(0,0)处于(0+mdX,0+mdY)）
			int mdX = r - TempWatchHandleImgPtr->w() * 0.5;
			int mdY = r - TempWatchHandleImgPtr->h() * 0.5;
			//当前图形绘制位置(x0,y0),计算偏移
			double arcx = cos(arc);
			double arcy = sin(arc);
			//针底到圆心的距离差
			int rf = (R - TempWatchHandleImgPtr->w()) / 2;
			int x0 = rx0 + (int)((double)r * arcx) - r - (int)((double)rf * arcx);
			int y0 = ry0 - (int)((double)r * arcy) - r + (int)((double)rf * arcy);
			Fl_Image *RGBimage = UI::RotatePic::NewRotatePic(TempWatchHandleImgPtr, whAngle);

			RGBimage->draw(x0 + FinX, y0 + FinY);
			UI::RotatePic::DeleteRotatePic(RGBimage);

			
		}
		//轴心
		{
#if 1
			TempAxisImgPtr->draw(FinX + model->DashboardConfig.Width/2 - TempAxisImgPtr->w() / 2, FinY + model->DashboardConfig.Height/2 - TempAxisImgPtr->h() / 2);
#else
			//变换后图像的正方形宽高
			long l = ::sqrt(1.0 * TempAxisImgPtr->w() * TempAxisImgPtr->w() + TempAxisImgPtr->h() * TempAxisImgPtr->h() );
			//自转(变换后的指针矩形区所在圆)半径
			long r = l * 0.5;
			//角度转换后的图形
			long newPicDataLen = l * l* 4;
			//当前图形绘制位置(x0,y0),计算偏移
			int x0 = model->DashboardConfig.Width/2-r;
			int y0 = model->DashboardConfig.Height/2-r;
			Fl_Image *RGBimage = UI::RotatePic::NewRotatePic(TempAxisImgPtr, whAngle);
			RGBimage->draw(x0 + FinX, y0 + FinY);
			UI::RotatePic::DeleteRotatePic(RGBimage);
#endif
		}


	}
	void DashboardView::InitArcLimit(DashboardModel * mode)
	{
		CurrentValue = 0;
		CurrentMaxValue = 100;
		CurrentMinValue = 0;
		MaxValueValid = true;
		UpperWarnValueValid = true;
		UpperDangerValueValid = true;
		if (!mode)
		{
			this->ArcStartAngle = mode->DashboardConfig.StartAngle + Default_Start_Angle;
			this->ArcEndAngle = mode->DashboardConfig.EndAngle + Default_Start_Angle;
			if (!mode->DashboardConfig.ArcDirection)
			{
				this->ArcStartAngle = -1 * this->ArcStartAngle;
				while (this->ArcStartAngle < 0)
				{
					this->ArcStartAngle += 360;
				}
				this->ArcEndAngle = -1 * this->ArcEndAngle + 360;
				while (this->ArcEndAngle < 0)
				{
					this->ArcEndAngle += 360;
				}
			}
#if KEEPWHOLECIRCLE
			if (mode->DashboardConfig.EndAngle < mode->DashboardConfig.StartAngle)
			{
				this->ArcEndAngle += 360;
			}
#endif
			FD = (ArcEndAngle - ArcStartAngle) / (CurrentMaxValue - CurrentMinValue)* -1;
			CurrentUpperWarnValue = mode->DashboardConfig.IsUpperWarnValueUsedRegister ? 80 : mode->DashboardConfig.UpperWarnValue;
			//当前下警报值
			CurrentLowerWarnValue = mode->DashboardConfig.IsLowerWarnValueUsedRegister ? 20 : mode->DashboardConfig.LowerWarnValue;
			CurrentUpperDangerValue = mode->DashboardConfig.IsUpperDangerUsedRegister ? 90 : mode->DashboardConfig.UpperDangerValue;
			//当前下危险值
			CurrentLowerDangerValue = mode->DashboardConfig.IsLowerDangerUsedRegister ? 10: mode->DashboardConfig.LowerDangerValue;
		}
		else
		{
			this->ArcStartAngle = 0 + Default_Start_Angle;
			this->ArcEndAngle = 360 + Default_Start_Angle;
			FD = (ArcEndAngle - ArcStartAngle) / (CurrentMaxValue - CurrentMinValue)*-1;
			this->CurrentUpperWarnValue = 80;
			this->CurrentLowerWarnValue = 20;
			this->CurrentUpperDangerValue = 90;
			this->CurrentLowerDangerValue = 10;

		}
	}
	void DashboardView::draw()
	{
		shared_ptr<DashboardControl> ctrl = BaseView.GetControl<DashboardControl>();
		shared_ptr<DashboardModel> model = BaseView.GetModel<DashboardModel>();
		FinX = model->DashboardConfig.X + model->DashboardConfig.OffX;
		FinY = model->DashboardConfig.Y + model->DashboardConfig.OffY;
		DrawStickChart(ctrl.get(), model.get());
	}
	void DashboardView::DrawStickChart(DashboardControl * ctrl, DashboardModel * model)
	{
		initvalue(model);
		initformat(model);
		loadimages(model);
		drawboard(model);
		drawgraduate(model);
		drawwathhandle(model);
	}
	void DashboardView::initvalue(DashboardModel* mode_)
	{
		if (mode_->DashboardConfig.ArcDirection)
		{
			this->ArcStartAngle = (mode_->DashboardConfig.StartAngle + Default_Start_Angle);
			this->ArcEndAngle = (mode_->DashboardConfig.EndAngle + Default_Start_Angle);
			/*while (this->ArcStartAngle < this->ArcEndAngle)
			{
				this->ArcStartAngle += 360;
			}*/
		}
		else
		{
			this->ArcEndAngle = -1 * mode_->DashboardConfig.StartAngle + Default_Start_Angle;
			this->ArcStartAngle = -1 * mode_->DashboardConfig.EndAngle + Default_Start_Angle;
			//while (this->ArcStartAngle > this->ArcEndAngle)
			//{
			//	this->ArcEndAngle += 360;
			//}
			/*this->ArcEndAngle = -1 * this->ArcEndAngle + 360;
			while (this->ArcEndAngle < 0)
			{
				this->ArcEndAngle += 360;
			}*/
		}
#if KEEPWHOLECIRCLE
		/*if (ArcEndAngle < ArcStartAngle)
		{
			this->ArcEndAngle += 360;
		}*/
#endif
		MaxValueValid = false;
		UpperWarnValueValid = false;
		UpperDangerValueValid = false;
		///算值
		//当前数值
		//mode_->m_DashboardConfig.curValue_ = DataService::IDataService::Ins()->GetNumber<int>(mode_->m_DashboardConfig.BarVar);
		CurrentValue = HmiMainMode?UIData::Number<double>(mode_->DashboardConfig.DataCtlSign):0;
		//当前最大值
		CurrentMaxValue = HmiMainMode ? mode_->DashboardConfig.MaxUseAddr ? UIData::Number<double>(mode_->DashboardConfig.MaxData) : mode_->DashboardConfig.MaxValue: mode_->DashboardConfig.MaxValue;
		//当前最小值
		CurrentMinValue = HmiMainMode ? mode_->DashboardConfig.MinUseAddr ? UIData::Number<double>(mode_->DashboardConfig.MinData) : mode_->DashboardConfig.MinValue : mode_->DashboardConfig.MinValue;
		MaxValueValid = CurrentMaxValue >= CurrentMinValue;
		if (MaxValueValid)
		{
			//限制范围
			CurrentValue = CurrentValue > CurrentMaxValue ? CurrentMaxValue : CurrentValue;
			//CurrentValue = CurrentValue < CurrentMinValue ? CurrentMinValue : CurrentValue;
			FD = (CurrentMaxValue > CurrentMinValue)?(ArcEndAngle - ArcStartAngle) / (CurrentMaxValue - CurrentMinValue)*(mode_->DashboardConfig.ArcDirection ? 1 : -1):0;
		}
		else
			//最大最小值不正确，取消绘制
			return;
		//范围报警
		if (mode_->DashboardConfig.IsUsedAlarmLimit)
		{
			//当前上警报值
			CurrentUpperWarnValue = HmiMainMode ? mode_->DashboardConfig.IsUpperWarnValueUsedRegister ? UIDataService::Ins().GetNumber<double>(mode_->DashboardConfig.UpperWarnRegister) : mode_->DashboardConfig.UpperWarnValue : mode_->DashboardConfig.UpperWarnValue;
			//当前下警报值
			CurrentLowerWarnValue = HmiMainMode ? mode_->DashboardConfig.IsLowerWarnValueUsedRegister ? UIDataService::Ins().GetNumber<double>(mode_->DashboardConfig.LowerWarnRegister) : mode_->DashboardConfig.LowerWarnValue : mode_->DashboardConfig.LowerWarnValue;
			UpperWarnValueValid = (CurrentUpperWarnValue >= CurrentLowerWarnValue) && (CurrentMaxValue >= CurrentUpperWarnValue) && (CurrentLowerWarnValue >= CurrentMinValue);
		}

		//危险报警
		if (mode_->DashboardConfig.IsUsedAlarmLimit)
		{
			//当前上危险值
			CurrentUpperDangerValue = HmiMainMode ? mode_->DashboardConfig.IsUpperDangerUsedRegister ? UIDataService::Ins().GetNumber<double>(mode_->DashboardConfig.UpperDangerRegister) : mode_->DashboardConfig.UpperDangerValue : mode_->DashboardConfig.UpperDangerValue;
			//当前下危险值
			CurrentLowerDangerValue = HmiMainMode ? mode_->DashboardConfig.IsLowerDangerUsedRegister ? UIDataService::Ins().GetNumber<double>(mode_->DashboardConfig.LowerDangerRegister) : mode_->DashboardConfig.LowerDangerValue : mode_->DashboardConfig.LowerDangerValue;
			UpperDangerValueValid = (CurrentUpperDangerValue >= CurrentLowerDangerValue) && (CurrentMaxValue >= CurrentUpperDangerValue) && (CurrentLowerDangerValue >= CurrentMinValue);
		}
	}
	void DashboardView::setlinestyle(DashboardModel * model)
	{
		fl_color(RGBColor(model->DashboardConfig.GraduateColor));
		fl_line_style(0, 2);
	}
	//根据角度和半径计算当前圆上的点相对于中心点的偏移位置（参考3点为0度，逆时针）
	//向右和向下偏移为正，向左和向上为负
	void DashboardView::calculatepospncircle(double angle, int radius, int& offsetX, int& offsetY)
	{
#if KEEPWHOLECIRCLE
		if (angle > 360)
		{
			angle = angle - 360;
		}
#endif
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
	

}


