#pragma once
#include "stdafx.h"
#include "BasicView.h"
namespace UI
{
	class GStickChartControl;
	class GStickChartModel;

	class GStickChartView : public HMIBaseView
	{
	public:
		GStickChartView(int X, int Y, int W, int H);
		~GStickChartView();
		BasicView BaseView;
	protected:
		void draw() override;
		void DrawStickChart(GStickChartControl* ctrl, GStickChartModel* model);
	private:
		//求正方形中，包含特定两个夹角所在的扇形区域的矩形区域，ptS为左上角，ptE为右下角
		//   ---------
		//	 |   |   |
		//   |   |   |
		//   | 1 | 0 |
		//   ---------
		//	 | 2 | 3 |
		//   |   |   |
		//   |   |   |
		//   ---------
		//分析象限的包含与存在标识
		void getarcrect(double startAngle, double endAngle, int& containquardences, int& existquardence);
		//返回坐标点在矩形所处的象限
		//#define QUADRANT1ST 0x01
		//#define QUADRANT2ND 0x02
		//#define QUADRANT3RD 0x04
		//#define QUADRANT4TH 0x08
		//#define CORCENTER	  0x10
		int getquaofpoint(int x, int y, int w, int h);
		bool judgeptifinside(int ptqua, int quafield);
		void calculatepospncircle(double angle, int radius, int& offsetX, int& offsetY);
		void initformat(GStickChartModel* model);
		void GetCurBarColor(GStickChartModel* model);
		//扇形样式
		void sectorstyle(GStickChartModel* model);
		//直条样式
		void straightstyle(GStickChartModel* model);
		void showpecent(GStickChartModel* model);
		//直条画图
		void getstraightfadeimgaera(Fl_Image* img,int fx,int fy,int fw,int fh);
		//full_为填充的容器坐标和长宽，b_为填充内容的当前坐标和长宽
		void calcustarightfillinfield(int direction, int fullx,int fully,int fullw,int fullh, bool useImg,int& bx, int& by, int& bw, int& bh);
		//扇形画图
		void getsectorfadeimgaera(Fl_Image* img,int radius,double angleS,double angleE);
	private:
		char GradFormat[20];//当前刻度格式 例：06.2f = 123.10
		char ChPecent[5];//当前刻度百分比
		Fl_Image* PtrBarImg;//常规棒图的背景（渐变、花纹等）
		Fl_Image* PtrUDBarImg;
		int BarX;
		int BarY;
		int FinX;
		int FinY;
	public:
		int CurValue_;//当前数值
		int CurMaxValue_; //当前最大值
		int CurMinValue_; //当前最小值
		int CurDstValue_; //当前目标值
		int CurRangeValue_;//当前误差范围
		int CurUpperValue_;//当前上警报值
		int CurLowerValue_;//当前下警报值
		int CurBarColor_; //当前棒色
		bool IsSpecialValue_;//当前是否为特殊值（上下危险区、上下警报、目标区间）
	};
}