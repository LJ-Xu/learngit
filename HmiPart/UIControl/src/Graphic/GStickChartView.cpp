#include "stdafx.h"
#include "UIComm.h"
#include "GStickChartView.h"
#include "GStickChartControl.h"
#include "GStickChartModel.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include <FL/Fl_Double_Window.H>
#include "AdvancedGraphic.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Output.H>
//#include<gl/glut.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "UIData.h"

#define QUADRANT1ST 0x01
#define QUADRANT2ND 0x02
#define QUADRANT3RD 0x04
#define QUADRANT4TH 0x08
#define CORCENTER	0x10
#define CALCULATESECTORRECT 0

namespace UI
{
	GStickChartView::GStickChartView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H), PtrBarImg(nullptr),BarX(-1),BarY(-1)
	{
		memset(GradFormat, 0, 20);

		CurValue_=50;//当前数值
		CurMaxValue_=100; //当前最大值
		CurMinValue_=0; //当前最小值
		CurDstValue_=30; //当前目标值
		CurRangeValue_=10;//当前误差范围
		CurUpperValue_=90;//当前上警报值
		CurLowerValue_=10;//当前下警报值
	}
	GStickChartView::~GStickChartView() 
	{
		/*if (TempLateImgPtr != nullptr)
			delete TempLateImgPtr;*/
		if (PtrBarImg != nullptr)
		{
			Fl_RGB_Image *rgbimage = (Fl_RGB_Image *)PtrBarImg;
			const uchar *data = rgbimage->array;
			delete PtrBarImg;
			delete[] data;
			PtrBarImg = nullptr;
		}
	}


	//const char NumTab[] = { '0', '1', '2', '3', '4' ,'5','6','7','8','9','%' };

	void GStickChartView::initformat(GStickChartModel* model)
	{
		if (GradFormat[0] != 0)
		{
			return;
		}
		int mainLen = (model->StickChartConfig.DecimalDigit > 0 ? model->StickChartConfig.DecimalDigit + 1 : 0) + model->StickChartConfig.IntegralDigit;
		if (mainLen > 0)
		{
			//snprintf(GradFormat, 20, "%%0%d.%df", mainLen, model->StickChartConfig.DecimalDigit);
			snprintf(GradFormat, 20, "%%0%d.%df", 1, model->StickChartConfig.DecimalDigit);
		}
	}
	//显示百分比
	void GStickChartView::showpecent(GStickChartModel* model)
	{
		//百分比
		if (model->StickChartConfig.ShowPercent)
		{
			memset(ChPecent, 0, 5);
			int pct = CurMaxValue_?( (float)CurValue_ / (float)CurMaxValue_) * 100:0;

			sprintf(ChPecent, "%d%%", pct);
			fl_font(model->StickChartConfig.TypeFaceGraduate.Font.Style, model->StickChartConfig.TypeFaceGraduate.Font.Size);
			/*设置字体颜色*/
			fl_color(fl_rgb_color(model->StickChartConfig.TypeFaceGraduate.RGBColor.R, model->StickChartConfig.TypeFaceGraduate.RGBColor.G, model->StickChartConfig.TypeFaceGraduate.RGBColor.B));
			fl_draw(ChPecent, (float)FinX + model->StickChartConfig.Width / 2, (float)FinY + model->StickChartConfig.Height / 2);
		}
	}

	void GStickChartView::getstraightfadeimgaera(Fl_Image * imgPre, int fx, int fy, int fw, int fh)
	{
		if (imgPre)
		{
			if (PtrBarImg!=nullptr)
			{
				Fl_RGB_Image *rgbimage = (Fl_RGB_Image *)PtrBarImg;
				const uchar *data = rgbimage->array;
				delete PtrBarImg;
				delete[] data;
				PtrBarImg = nullptr;
			}
			int maxX=fx+fw, maxY=fy+fh;
			bool fadeR = false, fadeC = false;
			//GStickChartModel * model = BaseView.GetModel<GStickChartModel>().get();
			//Fl_Image * img = imgPre->copy(model->StickChartConfig.Width, model->StickChartConfig.Height);
			Fl_Image * img = imgPre->copy(imgPre->w(), imgPre->h());
			//参考自GDynPicView.cpp L162，当位图不存在alpha值时赋予alpha值并产生一个带有alpha值的位图
			if (img->d() != 4)
			{
				uchar *rgb = new uchar[img->w() * img->h() * img->d()];
				uchar *rgba = new uchar[img->w() * img->h() * 4];
				memcpy(rgb, img->data()[0], img->w() * img->h() * img->d());
				long indexSrcPicPix=0,indexNewPicPix = 0;
				for (int curRow = 0; curRow <= img->h(); ++curRow)
				{
					fadeR = (curRow >= fy) && (curRow <= maxY);
					for (int curCol = 0; curCol <= img->w(); ++curCol)
					{
						if (fadeR)
						{
							fadeC = (curCol >= fx) && (curCol <= maxX);
						}
						rgba[indexNewPicPix++] = rgb[indexSrcPicPix++];
						rgba[indexNewPicPix++] = rgb[indexSrcPicPix++];
						rgba[indexNewPicPix++] = rgb[indexSrcPicPix++];
						rgba[indexNewPicPix++] = (fadeR&&fadeC)?0:255;
						fadeC = false;
						/*indexSrcPicPix+=3;
						indexNewPicPix+=4;*/
					}
					fadeR = false;
				}
				PtrBarImg = new Fl_RGB_Image(rgba, imgPre->w(), imgPre->h(), 4);
				delete[] rgb;
			}
			else
			{
				uchar *rgba = new uchar[img->w() * img->h() * 4];
				memcpy(rgba, img->data()[0], img->w() * img->h()* 4);
				int indexPicPix = 0;
				bool fade = false;
				for (int curRow = 0; curRow < img->h(); ++curRow)
				{
					fadeR = (curRow < fy) || (curRow > maxY);
					for (int curCol = 0; curCol < img->w(); ++curCol)
					{
						fadeC = (curCol < fx) || (curCol > maxX);
						if (fadeR||fadeC)
						{
							rgba[indexPicPix + 3] = 0;
							fadeC = false;
						}
						indexPicPix+=4;
					}
					fadeR = false;
				}
				PtrBarImg = new Fl_RGB_Image(rgba, imgPre->w(), imgPre->h(), 4);
			}
			//const uchar *data = rgbimage->array;
			//delete TempLateImgPtr;
			//delete[] data;
			delete img;
		}

	}
	void GStickChartView::calcustarightfillinfield(int direction,int fullx, int fully, int fullw, int fullh,bool useImg, int& bx, int& by, int& bw, int& bh)
	{
		float fenmu = CurMaxValue_ > CurMinValue_? (float)(CurMaxValue_ - CurMinValue_):0;
		switch (direction)
		{
			//向上延生
		case 0:
			bw = fullw;
			bh = (float)(CurValue_ - CurMinValue_) / fenmu*fullh;

			bx = fullx;
			if (useImg)
			{
				by = fully + (fullh - bh);
				//bh = fullh - bh;
			}
			else
				by = fully + (fullh - bh);
			break;
			//向下延生
		case 1:
			bw = fullw;
			bh = (float)(CurValue_ - CurMinValue_) / fenmu *fullh;

			bx = fullx;
			if (useImg)
			{
				by = fully;
				//bh = fullh - bh;
			}
			else
				by = fully;
			break;
			//向左延生
		case 2:
			bh = fullh;
			bw = (float)(CurValue_ - CurMinValue_) / fenmu*fullw;

			if (useImg)
			{
				bx = fullx + (fullw - bw);
				//bw = fullw - bw;
			}
			else
				bx = fullx + (fullw - bw);
			by = fully;
			break;
			//向右延生
		case 3:
			bh = fullh;
			bw = (float)(CurValue_ - CurMinValue_) / fenmu*fullw;

			if (useImg)
			{
				bx = fullx;
				//bw = fullw - bw;
			}
			else
				bx = fullx;
			by = fully;
			break;
		default:
			break;
		}
	}

	void GStickChartView::getarcrect(double startAngle, double endAngle, int &containquardences,int& existquardences)
	{
		//判断起始角度所在的米字分区(从0到3,每90度一个分区)
		//   ---------
		//	 |   |   |
		//   |   |   |
		//   | 1 | 0 |
		//   ---------
		//	 | 2 | 3 |
		//   |   |   |
		//   |   |   |
		//   ---------
		double stArc, edArc;
		stArc = (startAngle > 360? startAngle - 360 :startAngle) * M_PI / 180;
		edArc = (endAngle > 360 ? endAngle - 360 : endAngle) * M_PI / 180;
		//判断角度所在象限
		int startQuadrant = (int)(startAngle / 90.0)%4;
		bool startOnAxis = ((int)startAngle % 90) == 0;
		int endQuadrant = (int)(endAngle / 90.0)%4;
		bool endOnAxis = ((int)endAngle % 90) == 0;
		switch (startQuadrant)
		{
		case 0:
		{
			switch (endQuadrant)
			{
				case 0:
				{
					existquardences |= 0x01;
					if (edArc > stArc)
						containquardences |= 0x0e;
					else
						containquardences |= 0x00;
					break;
				}
				case 1:
				{
					existquardences |= 0x03;
					containquardences |= 0x00;
					break;
				}
				case 2:
				{
					existquardences |= 0x05;
					containquardences |= 0x02;
					break;
				}
				case 3:
				{
					existquardences |= 0x09;
					containquardences |= 0x06;
					break;
				}
			}
			break;
		}
		case 1:
		{
			switch (endQuadrant)
			{
			case 0:
			{
				existquardences |= 0x03;
				containquardences |= 0x0c;
				break;
			}
			case 1:
			{
				existquardences |= 0x02;
				if (edArc < stArc)
					containquardences |= 0x0d;
				else
					containquardences |= 0x00;
				break;
			}
			case 2:
			{
				existquardences |= 0x06;
				containquardences |= 0x00;
				break;
			}
			case 3:
			{
				existquardences |= 0x0a;
				containquardences |= 0x04;
				break;
			}
			}
			break;
		}
		case 2:
		{
			switch (endQuadrant)
			{
			case 0:
			{
				existquardences |= 0x05;
				containquardences |= 0x08;
				break;
			}
			case 1:
			{
				existquardences |= 0x06;
				containquardences |= 0x09;
				break;
			}
			case 2:
			{
				existquardences |= 0x04;
				if (edArc < stArc)
					containquardences |= 0x0b;
				else
					containquardences |= 0x00;
				break;
			}
			case 3:
			{
				existquardences |= 0x0c;
				containquardences |= 0x00;
				break;
			}
			}
			break;
		}
		case 3:
		{
			switch (endQuadrant)
			{
			case 0:
			{
				existquardences |= 0x09;
				containquardences |= 0x00;
				break;
			}
			case 1:
			{
				existquardences |= 0x0a;
				containquardences |= 0x01;
				break;
			}
			case 2:
			{
				existquardences |= 0x0b;
				containquardences |= 0x03;
				break;
			}
			case 3:
			{
				existquardences |= 0x08;
				if (edArc < stArc)
					containquardences |= 0x07;
				else
					containquardences |= 0x00;
				break;
			}
			}
			break;
		}
		}
	}

	int GStickChartView::getquaofpoint(int x, int y, int w, int h)
	{
		if ((x == w / 2) && (y == h / 2))
			return CORCENTER;
		if (x < w / 2)
		{
			//第二象限
			if (y <= h / 2)
			{
				return QUADRANT2ND;
			}
			//第三象限
			else
			{
				return QUADRANT3RD;
			}
		}
		else
		{
			//第四象限
			if (y >= h / 2)
			{
				return QUADRANT4TH;
			}
			//第一象限
			else
			{
				return QUADRANT1ST;
			}
		}
	}

	bool GStickChartView::judgeptifinside(int ptqua, int quafield)
	{
		return (ptqua& quafield)>0;
	}

	//扇形局部透明
	void GStickChartView::getsectorfadeimgaera(Fl_Image * imgPre, int radius,double angleS, double angleE)
	{
		if (imgPre)
		{
			if (PtrBarImg != nullptr)
			{
				Fl_RGB_Image *rgbimage = (Fl_RGB_Image *)PtrBarImg;
				const uchar *data = rgbimage->array;
				delete PtrBarImg;
				delete[] data;
				PtrBarImg = nullptr;
			}
			/*stArc = (startAngle > 360 ? startAngle - 360 : startAngle) * M_PI / 180;
			edArc = (endAngle > 360 ? endAngle - 360 : endAngle) * M_PI / 180;*/
			double stArcT = angleS * M_PI / 180;
			double edArcT = angleE * M_PI / 180;
			int containquardences = 0, existquardence = 0;
			this->getarcrect(angleS, angleE, containquardences, existquardence);
			GStickChartModel * model = BaseView.GetModel<GStickChartModel>().get();
			//Fl_Image * img = imgPre->copy(radius, radius);
			Fl_Image * img = imgPre;
			//以中心点为轴分析扇面所占的矩形区间
			int ptqua = -1;
			bool fade = false;
			int r = img->w() / 2;
			double k = 0.0;
			if (img->d() != 4)
			{
				uchar *rgb = new uchar[img->w() * img->h() * img->d()];
				uchar *rgba = new uchar[img->w() * img->h() * 4];
				memcpy(rgb, img->data()[0], img->w() * img->h() * img->d());
				long indexSrcPicPix = 0, indexNewPicPix = 0;
				for (int y = 0; y <= img->h(); ++y)
				{
					for (int x = 0; x <= img->w(); ++x)
					{
						ptqua = getquaofpoint(x, y, img->w(), img->h());
						if (judgeptifinside(ptqua, containquardences))
						{
							fade = false;
						}
						else
						{
							if (judgeptifinside(ptqua, existquardence))
							{
								k = atan((double)(r - y) / ((x - r) == 0 ? 1.0 : (double)(x - r)));
								switch (ptqua)
								{
								case 2:
									k = M_PI + k;
									break;
								case 4:
									k = M_PI + k;
									break;
								case 8:
									k = M_PI * 2 + k;
									break;
								}
								if ((k > stArcT) && (k < edArcT))
								//if ((atan(y / x) > stArcT) && (atan(y / x) < edArcT))
								{
									fade = false;
								}
							}
							
						}
						if (fade)
						{
							rgba[indexNewPicPix++] = rgb[indexSrcPicPix++];
							rgba[indexNewPicPix++] = rgb[indexSrcPicPix++];
							rgba[indexNewPicPix++] = rgb[indexSrcPicPix++];
							rgba[indexNewPicPix++] = 0;
						}
						else
						{
							fade = true;
							indexNewPicPix += 4;
							indexSrcPicPix += 3;
						}
					}
				}
				PtrBarImg = new Fl_RGB_Image(rgba, radius, radius, 4);
				delete[] rgb;
			}
			else
			{
				uchar *rgba = new uchar[img->w() * img->h() * 4];
				memcpy(rgba, img->data()[0], img->w() * img->h() * 4);
				int indexPicPix = 0;
				bool fade = true;
				for (int y = 0; y < img->h(); ++y)
				{
					for (int x = 0; x < img->w(); ++x)
					{
						ptqua = getquaofpoint(x, y, img->w(), img->h());
						if (judgeptifinside(ptqua, containquardences))
						{
							fade = false;
						}
						else
						{
							if (judgeptifinside(ptqua, existquardence))
							{
								k = atan((double)(r-y) / ((x-r) == 0 ? 1.0 : (double)(x-r)));
								switch (ptqua)
								{
								case 2:
									k = M_PI + k;
									break;
								case 4:
									k = M_PI + k;
									break;
								case 8:
									k = M_PI * 2 + k;
									break;
								}
								if ((k > stArcT) && (k < edArcT))
								//if ((atan(y / (x==0?1:x)) > stArc) && (atan(y / (x == 0 ? 1 : x)) < edArc))
								{
									fade = false;
								}
							}
						}
						if (fade)
						{
							rgba[indexPicPix + 3] = 0;
						}
						else
						{
							fade = true;
						}
						indexPicPix += 4;
					}
				}
				PtrBarImg = new Fl_RGB_Image(rgba, img->w() , img->h(), 4);
			}
			//const uchar *data = rgbimage->array;
			//delete TempLateImgPtr;
			//delete[] data;
			//delete img;
		}
	}

	

	//根据角度和半径计算当前圆上的点相对于中心点的偏移位置（参考3点为0度，逆时针）
	//向右和向下偏移为正，向左和向上为负
	void GStickChartView::calculatepospncircle(double angle, int radius, int& offsetX, int& offsetY)
	{
		if (angle > 360.0f)
		{
			angle = angle - 360.0f;
		}
		double arc = angle * M_PI / 180.0f;
		if (0.0f <= angle && 90.0f > angle)
		{
			offsetX = (int)(cos(arc)*(double)radius);
			offsetY = (int)(-sin(arc)*(double)radius);
		}
		else if (90.0f <= angle && 180.0f > angle)
		{
			offsetX = -(int)(cos(M_PI - arc)*(double)radius);
			offsetY = -(int)(sin(M_PI - arc)*(double)radius);
		}
		else if (180.0f <= angle && 270.0f > angle)
		{
			offsetX = -(int)(sin(M_PI * 3.0f / 2.0f - arc)*(double)radius);
			offsetY = (int)(cos(M_PI * 3.0f / 2.0f - arc)*(double)radius);
		}
		else
		{
			offsetX = (int)(cos(M_PI * 2.0f - arc)*(double)radius);
			offsetY = (int)(sin(M_PI * 2.0f - arc)*(double)radius);
		}
	}
	//分析获取当前的主要填充色（默认/高危/低危/目标达成）
	void GStickChartView::GetCurBarColor(GStickChartModel* model)
	{
		//当前应显示的填充色
		CurBarColor_ = model->StickChartConfig.FIllinColor;
		//报警色
		if (model->StickChartConfig.UseRangeWarn)
		{
			if (CurValue_ >= CurUpperValue_)
			{
				CurBarColor_ = model->StickChartConfig.UpperWarnFillinColor;
			}
			else if (CurValue_ <= CurLowerValue_)
			{
				CurBarColor_ = model->StickChartConfig.LowerWarnFillinColor;
			}
		}
		//目标色覆盖报警色
		if (model->StickChartConfig.UseDstField)
		{
			if ((CurValue_ <= CurDstValue_ + CurRangeValue_) && (CurValue_ >= CurDstValue_ - CurRangeValue_))
			{
				CurBarColor_ = model->StickChartConfig.DstColor;
			}
		}
	}

	void SetLineStyle(GStickChartModel* model)
	{
		fl_color(RGBColor(model->StickChartConfig.LineColor));
		fl_line_style(model->StickChartConfig.LineStyle, model->StickChartConfig.LineWidget);
	}

	//直条样式
	//棒身高度
	// -----
	//|		|
	//|-----|
	//|/////|←高度
	// -----
	void GStickChartView::straightstyle(GStickChartModel* model)
	{
		bool bUseImage = model->StickChartConfig.UseFillinShade || model->StickChartConfig.UseFillinStyle;
		BarX = FinX;
		BarY = FinY;
		int BarWidth = model->StickChartConfig.Width;
		int BarHeight = model->StickChartConfig.Height;
		

		//事前计算(绘制区域)
		int curBarWidth, curBarHeight, curX, curY;
		if (!model->StickChartConfig.IsUserDefineBar)
		{
			Fl_Image* flImgPtr = IResourceService::Ins()->GetImage(model->StickChartConfig.BarRes.KeyVal);
			if (flImgPtr)
			{
				//bool bUseImage = model->StickChartConfig.UseFillinShade || model->StickChartConfig.UseFillinStyle;
				BarX += model->StickChartConfig.BarAreaRectanglePointX;
				BarY += model->StickChartConfig.BarAreaRectanglePointY;
				BarWidth = model->StickChartConfig.BarAreaRectangleSizeWidth;
				BarHeight = model->StickChartConfig.BarAreaRectangleSizeHeight;
				//计算区域
				calcustarightfillinfield(model->StickChartConfig.BarDirection, BarX, BarY, BarWidth, BarHeight, model->StickChartConfig.UseFillinShade, curX, curY, curBarWidth, curBarHeight);
				//先画背景
				flImgPtr->draw(FinX, FinY, model->StickChartConfig.Width, model->StickChartConfig.Height);

				//绘制液体区
				if (bUseImage)//渐变色或花纹
				{
					Fl_Image* flImgPtr = IResourceService::Ins()->GetImage(model->StickChartConfig.LinearGradientRes.KeyVal);
					getstraightfadeimgaera(flImgPtr, curX - BarX, curY - BarY, curBarWidth, curBarHeight);
					//Fl_Image* flImgPtr = IResourceService::Ins()->GetImage(model->StickChartConfig.LinearGradientRes.KeyVal)->copy(curBarWidth, curBarHeight);
					//getstraightfadeimgaera(flImgPtr, 0, 0, curBarWidth, curBarHeight);
					//delete flImgPtr;
					if (PtrBarImg != nullptr)
					{
						//image(PtrBarImg);
						PtrBarImg->draw(BarX, BarY);
						align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
						box(FL_NO_BOX);
						draw_box();
					}
				}
				else
				{

					fl_rectf(curX, curY, curBarWidth, curBarHeight, RGBColor(CurBarColor_));
				}

			}
		}
		else
		{
			//绘制背景
			if (model->StickChartConfig.ShowBackground)
			{
				fl_rectf(FinX,
					FinY,
					model->StickChartConfig.Width,
					model->StickChartConfig.Height,
					RGBColor(model->StickChartConfig.ColorBack));
			}
			//计算填充区域

			//bool bUseImage = model->StickChartConfig.UseFillinShade || model->StickChartConfig.UseFillinStyle;
			//
			int spaceleft=0,spacetop=0,spacewidth = 0, spaceheight = 0;
			if (model->StickChartConfig.ShowGraduate)
			{
				switch (model->StickChartConfig.BarDirection)
				{
				case 0:
				case 1:
					if (!model->StickChartConfig.RelatedGraduateSeat)
					{
						spaceleft = model->StickChartConfig.MainGraduateLen;
					}
					spacewidth = model->StickChartConfig.MainGraduateLen;
					break;
				case 2:
				case 3:
					if (!model->StickChartConfig.RelatedGraduateSeat)
					{
						spacetop = model->StickChartConfig.MainGraduateLen;
					}
					spaceheight = model->StickChartConfig.MainGraduateLen;
					break;
				}
			}
			//给刻度数字留空间
			if (model->StickChartConfig.ShowGraduateSign)
			{
				switch (model->StickChartConfig.BarDirection)
				{
				case 0:
				case 1:
					if (!model->StickChartConfig.RelatedGraduateSeat)
					{
						spaceleft += model->StickChartConfig.TypeFaceGraduate.Font.Size*(model->StickChartConfig.IntegralDigit+ model->StickChartConfig.DecimalDigit)/2;
					}
					spacewidth += model->StickChartConfig.TypeFaceGraduate.Font.Size*(model->StickChartConfig.IntegralDigit + model->StickChartConfig.DecimalDigit) / 2;
					//spaceheight += model->StickChartConfig.TypeFaceGraduate.Font.Size/2;
					break;
				case 2:
				case 3:
					//spaceleft += model->StickChartConfig.TypeFaceGraduate.Font.Size*(model->StickChartConfig.IntegralDigit + model->StickChartConfig.DecimalDigit) / 4;
					if (!model->StickChartConfig.RelatedGraduateSeat)
					{
						spacetop += model->StickChartConfig.TypeFaceGraduate.Font.Size;
					}
					spaceheight += model->StickChartConfig.TypeFaceGraduate.Font.Size;
					break;
				}
			}
			BarX += spaceleft;
			BarY += spacetop;
			BarWidth -= spacewidth;
			BarHeight -= spaceheight;

			calcustarightfillinfield(model->StickChartConfig.BarDirection, BarX, BarY, BarWidth, BarHeight, bUseImage, curX, curY, curBarWidth, curBarHeight);
			//绘制液体区
			if (bUseImage)//渐变色或花纹
			{
				Fl_Image* flImgPtr = IResourceService::Ins()->GetImage(model->StickChartConfig.LinearGradientRes.KeyVal);
				getstraightfadeimgaera(flImgPtr, curX - FinX, curY - FinY, curBarWidth, curBarHeight);
				if (PtrBarImg != nullptr)
				{
					image(PtrBarImg);
					align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
					box(FL_NO_BOX);
					draw_box();
				}
			}
			else
			{
				fl_rectf(curX, curY, curBarWidth, curBarHeight, RGBColor(CurBarColor_));
			}
		}


		//绘制刻度
		if (model->StickChartConfig.ShowGraduate)
		{
			//临时的运算变量，通常为刻度起点，否则为平行坐标
			int tpMX= BarX, tpMY=BarY, tpSX= BarX, tpSY= BarY;
			//部分不变值的预算
			switch (model->StickChartConfig.BarDirection)
			{
			case 0:
			case 1:
				switch (model->StickChartConfig.RelatedGraduateSeat)
				{
				case 0:
					tpMX = BarX - model->StickChartConfig.MainGraduateLen;
					if (model->StickChartConfig.ShowAxis)
					{
						fl_color(RGBColor(model->StickChartConfig.LineColor));
						fl_line_style(model->StickChartConfig.LineStyle, model->StickChartConfig.LineWidget);
						fl_line(BarX, tpMY, BarX, tpMY + BarHeight);
					}
					break;
				case 1:
					tpMX = BarX + BarWidth;
					if (model->StickChartConfig.ShowAxis)
					{
						fl_color(RGBColor(model->StickChartConfig.LineColor));
						fl_line_style(model->StickChartConfig.LineStyle, model->StickChartConfig.LineWidget);
						fl_line(tpMX, tpMY, tpMX, tpMY + BarHeight);
					}
					break;
				}
				
				break;
			case 2:
			case 3:
				switch (model->StickChartConfig.RelatedGraduateSeat)
				{
				case 0:
					tpMY = BarY - model->StickChartConfig.MainGraduateLen;
					if (model->StickChartConfig.SecondaryGraduateCount > 0)
						tpSY = BarY - model->StickChartConfig.SecondaryGraduateLen;
					if (model->StickChartConfig.ShowAxis)
					{
						fl_color(RGBColor(model->StickChartConfig.LineColor));
						fl_line_style(model->StickChartConfig.LineStyle, model->StickChartConfig.LineWidget);
						fl_line(tpMX, BarY, tpMX + BarWidth, BarY);
					}
					break;
				case 1:
					tpMY = BarY + BarHeight;
					if (model->StickChartConfig.SecondaryGraduateCount > 0)
						tpSY = tpMY;
					if (model->StickChartConfig.ShowAxis)
					{
						fl_color(RGBColor(model->StickChartConfig.LineColor));
						fl_line_style(model->StickChartConfig.LineStyle, model->StickChartConfig.LineWidget);
						fl_line(tpMX, tpMY, tpMX + BarWidth, tpMY);
					}
					break;
				}
				break;
			}

			int sx = tpMX, sy = tpMY;
			bool ishor = false;
			int len = 0;
#if 1
			if (model->StickChartConfig.BarDirection == 0 || model->StickChartConfig.BarDirection == 1)
			{
				//len = model->StickChartConfig.Height;
				len = BarHeight;
			}
			else
			{
				//len = model->StickChartConfig.Width;
				len = BarWidth;
				ishor = true;
			}
#else
			if (!model->StickChartConfig.IsUserDefineBar)
			{
				len = BarHeight;
			}
			else
			{
				if (model->StickChartConfig.BarDirection == 0 || model->StickChartConfig.BarDirection == 1)
				{
					len = model->StickChartConfig.Height;
				}
				else
				{
					len = model->StickChartConfig.Width;
					ishor = true;
				}
			}
#endif
			
			AdvancedGarphic::DrawScale(ishor, model->StickChartConfig.RelatedGraduateSeat, sx, sy, len,model->StickChartConfig.MainGraduateCount,model->StickChartConfig.SecondaryGraduateCount, model->StickChartConfig.MainGraduateLen, model->StickChartConfig.SecondaryGraduateLen, model->StickChartConfig.LineStyle, model->StickChartConfig.LineColor, model->StickChartConfig.LineWidget);

			//fl_line_style(FL_SOLID);

			//数字标记
			if ((model->StickChartConfig.ShowGraduateSign) && (model->StickChartConfig.MainGraduateCount > 1) && GradFormat[0] != '0')
			{
				int dx = tpMX, dy = tpMY;
				int fontoffset = model->StickChartConfig.TypeFaceGraduate.Font.Size * (model->StickChartConfig.IntegralDigit + model->StickChartConfig.DecimalDigit) / 2;
				int len = 0;
				if (model->StickChartConfig.BarDirection == 0 || model->StickChartConfig.BarDirection == 1)
				{
					len = BarHeight;
					switch (model->StickChartConfig.RelatedGraduateSeat)
					{
					case 0:
						dx = tpMX - fontoffset;
						dy = tpMY;// -model->StickChartConfig.TypeFaceGraduate.Font.Size / 4;
						break;
					case 1:
						dx = tpMX + model->StickChartConfig.MainGraduateLen;
						dy = tpMY;// -model->StickChartConfig.TypeFaceGraduate.Font.Size / 4;
						break;
					}
				}
				else
				{
					len = BarWidth;
					switch (model->StickChartConfig.RelatedGraduateSeat)
					{
					case 0:
						dx = tpMX ;
						dy = tpMY;// -model->StickChartConfig.TypeFaceGraduate.Font.Size / 4;
						break;
					case 1:
						dx = tpMX;
						dy = tpMY + model->StickChartConfig.MainGraduateLen+ model->StickChartConfig.TypeFaceGraduate.Font.Size;
						break;
					}
				}
				int direct = model->StickChartConfig.BarDirection;
#if 1
				/*len = BarHeight;
				direct = direct % 2;*/
#else
				if (!model->StickChartConfig.IsUserDefineBar)
				{
					len = BarHeight;
					direct = direct % 2;
				}
#endif
				AdvancedGarphic::DrawScaleMark(direct,dx, dy, len, model->StickChartConfig.TypeFaceGraduate, model->StickChartConfig.MainGraduateCount, (float)(CurMinValue_), (float)(CurMaxValue_), GradFormat, model->StickChartConfig.IntegralDigit);
			}
		}
		//百分比
		showpecent(model);

		//边框
		if (model->StickChartConfig.ShowFrame)
		{
			fl_color(RGBColor(model->StickChartConfig.ColorShowFrame));
			fl_rect(BarX,
				BarY,
				BarWidth,
				BarHeight);
		}
	}

	//扇形样式
	void GStickChartView::sectorstyle(GStickChartModel* model)
	{
		bool bUseImg = model->StickChartConfig.UseFillinShade || model->StickChartConfig.UseFillinStyle;
		BarX = FinX;
		BarY = FinY;
		int curdiam = model->StickChartConfig.Width <
			model->StickChartConfig.Height ? model->StickChartConfig.Width : model->StickChartConfig.Height ;
		int curRadius = curdiam / 2;
		if (model->StickChartConfig.ShowGraduate&&(!model->StickChartConfig.RelatedGraduateSeat)&&!bUseImg)
		{
			curRadius -= model->StickChartConfig.MainGraduateLen;
			BarX += model->StickChartConfig.MainGraduateLen;
			BarY += model->StickChartConfig.MainGraduateLen;
			if (model->StickChartConfig.ShowGraduateSign)
			{
				curRadius -= model->StickChartConfig.TypeFaceGraduate.Font.Size;
				BarX += model->StickChartConfig.TypeFaceGraduate.Font.Size;
				BarY += model->StickChartConfig.TypeFaceGraduate.Font.Size;
			}
		}

		//针对fltk修正后的起始和终点角度
		double realStartAngle, realEndAngle;
		//棒区比例
		double curValuePecent = CurMaxValue_> CurMinValue_?(float)(CurValue_ - CurMinValue_) / (float)(CurMaxValue_ - CurMinValue_):0;
		double curSweepAngle = curValuePecent * model->StickChartConfig.ArcSweepAngle;
		//棒区角度
		double curStartAngle, curEndAnlge;

		switch (model->StickChartConfig.ArcDirection)
		{
		case 0:
			if (model->StickChartConfig.ArcStartAngle > model->StickChartConfig.ArcSweepAngle)
			{
				realEndAngle = model->StickChartConfig.ArcStartAngle;
				realStartAngle = realEndAngle - model->StickChartConfig.ArcSweepAngle;
			}
			else
			{
				realStartAngle = model->StickChartConfig.ArcStartAngle - model->StickChartConfig.ArcSweepAngle + 360.0;
				realEndAngle = model->StickChartConfig.ArcStartAngle + 360.0;
			}
			if (model->StickChartConfig.ArcStartAngle > curSweepAngle)
			{
				curEndAnlge = model->StickChartConfig.ArcStartAngle;
				curStartAngle = curEndAnlge - curSweepAngle;
			}
			else
			{
				curStartAngle = model->StickChartConfig.ArcStartAngle - curSweepAngle + 360.0;
				curEndAnlge = model->StickChartConfig.ArcStartAngle + 360.0;
			}
			break;
		case 1:
			realStartAngle = model->StickChartConfig.ArcStartAngle;
			realEndAngle = realStartAngle + model->StickChartConfig.ArcSweepAngle;
			curStartAngle = model->StickChartConfig.ArcStartAngle;
			curEndAnlge = curStartAngle + curSweepAngle;
			break;
		}
		//绘制背景
		if (model->StickChartConfig.ShowBackground||1)
		{

			fl_begin_polygon();
			fl_color(RGBColor(model->StickChartConfig.ColorBack));
			fl_pie(BarX,
				BarY,
				curRadius*2, curRadius*2,
				realStartAngle, realEndAngle);
			fl_end_polygon();
		}
		//绘制液体区
		if (bUseImg)//渐变色
		{
			Fl_Image* flImgPtr = IResourceService::Ins()->GetImage(model->StickChartConfig.LinearGradientRes.KeyVal);
			getsectorfadeimgaera(flImgPtr, curdiam,curStartAngle,curEndAnlge);
			if (PtrBarImg != nullptr)
			{
				//delete rotatepic;
				//PtrBarImg->draw(BarX,BarY);
				/*delete newImage;
				delete[] rotatergba;*/
				image(PtrBarImg);
				align(FL_ALIGN_IMAGE_BACKDROP);
				box(FL_NO_BOX);
				draw_box();
			}
		}
		else
		{
			fl_begin_polygon();
			fl_color(RGBColor(CurBarColor_));
			fl_pie(BarX,
				BarY,
				curRadius * 2, curRadius * 2,
				curStartAngle, curEndAnlge);
			fl_end_polygon();

		}
		//中心点坐标
		int centerX = BarX + curRadius;
		int centerY = BarY + curRadius;
		//内环
		if (model->StickChartConfig.ArcInnerCirclePercent)
		{
			int rInside = (int)((float)(model->StickChartConfig.ArcInnerCirclePercent*curRadius) / 100.f);
			fl_begin_polygon();
			fl_color(RGBColor(model->StickChartConfig.ColorBack));
			fl_pie(centerX - rInside,
				centerY - rInside,
				rInside*2, rInside*2,
				realStartAngle, realEndAngle);
			fl_end_polygon();
		}

		//绘制刻度
		if (model->StickChartConfig.ShowGraduate)
		{
			if (model->StickChartConfig.ShowAxis)
			{
				fl_color(RGBColor(model->StickChartConfig.LineColor));
				fl_line_style(model->StickChartConfig.LineStyle, model->StickChartConfig.LineWidget);
				fl_arc(BarX,
					BarY,
					curRadius * 2,
					curRadius * 2, realStartAngle, realEndAngle);
			}
			//主刻度和副刻度各自的角度间隔（角度）
			double mOffset = 0;
			double sOffset = 0;

			if (model->StickChartConfig.MainGraduateCount > 1)
			{
				mOffset = (float)model->StickChartConfig.ArcSweepAngle / (float)(model->StickChartConfig.MainGraduateCount - 1);

				if (model->StickChartConfig.SecondaryGraduateCount > 0)
				{
					sOffset = (float)mOffset / (float)(model->StickChartConfig.SecondaryGraduateCount + 1);
				}
				//角分坐标
				double curLineAngle = realStartAngle;
				//当前坐标
				int lineStartX, lineStartY;
				int lineEndX, lineEndY;
				//int fontCX, fontCY;
				//线段收尾坐标的相对圆心的半径长度，根据内环和外环时有不同的延生方向
				int curMainLineEndPtRadius, curSecLineEndPtRadius;
				//数字标记坐标的中心点相对圆心的半径长度，区分情况同上
				int curGraduatePtRadius;
				switch (model->StickChartConfig.RelatedGraduateSeat)
				{
				case 0:
					curMainLineEndPtRadius = curRadius + model->StickChartConfig.MainGraduateLen;
					if (model->StickChartConfig.SecondaryGraduateCount > 0)
					{
						curSecLineEndPtRadius = curRadius + model->StickChartConfig.SecondaryGraduateLen;
					}
					if (model->StickChartConfig.ShowGraduateSign)
					{
						curGraduatePtRadius = curMainLineEndPtRadius + model->StickChartConfig.TypeFaceGraduate.Font.Size / 2;
					}
					break;
				case 1:
					curMainLineEndPtRadius = curRadius - model->StickChartConfig.MainGraduateLen;

					if (model->StickChartConfig.SecondaryGraduateCount > 0)
					{
						curSecLineEndPtRadius = curRadius - model->StickChartConfig.SecondaryGraduateLen;
					}
					if (model->StickChartConfig.ShowGraduateSign)
					{
						curGraduatePtRadius = curMainLineEndPtRadius - model->StickChartConfig.TypeFaceGraduate.Font.Size / 2;
					}
					break;
				}

				int curSAngle = 0;
				//线段样式
				SetLineStyle(model);
				//画线
				for (int idxM = 0; idxM < model->StickChartConfig.MainGraduateCount; ++idxM)
				{
					calculatepospncircle(curLineAngle, curRadius, lineStartX, lineStartY);
					calculatepospncircle(curLineAngle, curMainLineEndPtRadius, lineEndX, lineEndY);
					fl_line(centerX + lineStartX, centerY + lineStartY, centerX + lineEndX, centerY + lineEndY);

					if (idxM < model->StickChartConfig.MainGraduateCount - 1)
					{
						for (int idxS = 1; idxS <= model->StickChartConfig.SecondaryGraduateCount; ++idxS)
						{
							curSAngle = curLineAngle + idxS * sOffset;
							calculatepospncircle(curSAngle, curRadius, lineStartX, lineStartY);
							calculatepospncircle(curSAngle, curSecLineEndPtRadius, lineEndX, lineEndY);
							fl_line(centerX + lineStartX, centerY + lineStartY, centerX + lineEndX, centerY + lineEndY);
						}
					}
					curLineAngle += mOffset;
				}
				if (model->StickChartConfig.ShowGraduateSign)
				{
					double startAngle = realStartAngle, sweepAngle = realEndAngle-startAngle;
					AdvancedGarphic::DrawScaleMark(centerX,centerY, model->StickChartConfig.TypeFaceGraduate, curGraduatePtRadius, model->StickChartConfig.MainGraduateCount, (float)(CurMinValue_), (float)(CurMaxValue_), startAngle, sweepAngle, GradFormat, model->StickChartConfig.IntegralDigit);
				}
			}
		}


		//百分比
		showpecent(model);
		//轴线

		//边框
		if (model->StickChartConfig.ShowFrame)
		{
			fl_begin_line();
			fl_color(RGBColor(model->StickChartConfig.ColorShowFrame));
			fl_arc(BarX,
				BarY,
				curRadius*2,
				curRadius*2, realStartAngle, realEndAngle);
			if (360!= (int)(realEndAngle- realStartAngle))
			{
				double stArc = realStartAngle * M_PI / 180;
				double edArc = realEndAngle * M_PI / 180;
				int sy = centerY - sin(stArc)*curRadius;
				int sx = centerX + cos(stArc)*curRadius;
				int ey = centerY - sin(edArc)*curRadius;
				int ex = centerX + cos(edArc)*curRadius;

				fl_line(centerX, centerY, sx, sy);
				fl_line(centerX, centerY, ex, ey);
			}
			
			fl_end_line();

		}

	}

	void GStickChartView::DrawStickChart(GStickChartControl* ctrl, GStickChartModel* model)
	{
		//fl_color(RGBColor(model->m_StickChartConfig.BackColor));
		//fl_line_style(0, 3);
		initformat(model);
		//getcurrentvalue(model);
		GetCurBarColor(model);
		//形状
		switch (model->StickChartConfig.Shape)
		{
			//直条
		case 0:
		{
			straightstyle(model);
			break;
		}
		//扇形
		case 1:
		{
			sectorstyle(model);
			break;
		}

		}


	}
	void GStickChartView::draw()
	{
		shared_ptr<GStickChartControl> ctrl = BaseView.GetControl<GStickChartControl>();
		shared_ptr<GStickChartModel> model = BaseView.GetModel<GStickChartModel>();
		FinX = model->StickChartConfig.X + model->StickChartConfig.OffX;
		FinY = model->StickChartConfig.Y + model->StickChartConfig.OffY;
		DrawStickChart(ctrl.get(), model.get());
	}
}
