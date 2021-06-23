#pragma once
#include "stdafx.h"
#include "BasicView.h"
namespace UI
{
	class DashboardControl;
	class DashboardModel;

	class DashboardView : public HMIBaseView
	{
	public:
		DashboardView(int X, int Y, int W, int H);
		~DashboardView();
		BasicView BaseView;
		const double PI = 3.1415926535897932384626433832795;
	public:
		void InitArcLimit(DashboardModel* mode);
	protected:
		void draw() override;
		void DrawStickChart(DashboardControl* ctrl, DashboardModel* model);
	private:
		void initvalue(DashboardModel* mode);
		void setlinestyle(DashboardModel* model);
		void calculatepospncircle(double angle, int radius, int& offsetX, int& offsetY);
		void drawrotateimage(DashboardModel* model,double angle);
		void loadimages(DashboardModel* model);//加载图片
		void initformat(DashboardModel* model);//刻度表达式
		void drawboard(DashboardModel* model);//绘制表盘（背景与危险色）
		void drawwathhandle(DashboardModel* model);//绘制表针与轴心
		void drawgraduate(DashboardModel* model);//绘制刻度(和数字标记)
		void drawsector(int cx,int cy,int r,int w,double sA,double eA,int color);
	private:
		Fl_Image* DashBoardImgPtr;//表盘图片
		Fl_Image* TempWatchHandleImgPtr;//表针绘制图片的原始模板
		Fl_Image* TempAxisImgPtr;//轴心临时绘制图片
		uchar* WatchHandleBuffer;//指针图片数据缓存
		uchar* AxisBuffer;//轴心图片数据缓存
		char ChPecent[5];//当前刻度百分比
		char GradFormat[20];//当前刻度格式 例：06.2f = 123.10
		int ArrNormalFieldAngleGroup[4];//正常颜色的范围区间，最多两个区间
	public:
		bool HmiMainMode;//上位机模式（不读取地址）
		bool MaxValueValid;//当最大值大于最小值时仪表盘有效显示
		bool UpperWarnValueValid;
		bool UpperDangerValueValid;
		int FinX;
		int FinY;
		double FD;//当前角度与值的换算关系,FD代表值对应角度的增长趋势
		float ArcStartAngle;//绘制区间的起始点，一定小于终点
		float ArcEndAngle;//绘制区间的终点，一定大于起始点
		double CurrentValue;
		double CurrentMaxValue;
		double CurrentMinValue;
		double CurrentUpperWarnValue;
		double CurrentLowerWarnValue;
		double CurrentUpperDangerValue;
		double CurrentLowerDangerValue;
	};
}


