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
		void loadimages(DashboardModel* model);//����ͼƬ
		void initformat(DashboardModel* model);//�̶ȱ��ʽ
		void drawboard(DashboardModel* model);//���Ʊ��̣�������Σ��ɫ��
		void drawwathhandle(DashboardModel* model);//���Ʊ���������
		void drawgraduate(DashboardModel* model);//���ƿ̶�(�����ֱ��)
		void drawsector(int cx,int cy,int r,int w,double sA,double eA,int color);
	private:
		Fl_Image* DashBoardImgPtr;//����ͼƬ
		Fl_Image* TempWatchHandleImgPtr;//�������ͼƬ��ԭʼģ��
		Fl_Image* TempAxisImgPtr;//������ʱ����ͼƬ
		uchar* WatchHandleBuffer;//ָ��ͼƬ���ݻ���
		uchar* AxisBuffer;//����ͼƬ���ݻ���
		char ChPecent[5];//��ǰ�̶Ȱٷֱ�
		char GradFormat[20];//��ǰ�̶ȸ�ʽ ����06.2f = 123.10
		int ArrNormalFieldAngleGroup[4];//������ɫ�ķ�Χ���䣬�����������
	public:
		bool HmiMainMode;//��λ��ģʽ������ȡ��ַ��
		bool MaxValueValid;//�����ֵ������Сֵʱ�Ǳ�����Ч��ʾ
		bool UpperWarnValueValid;
		bool UpperDangerValueValid;
		int FinX;
		int FinY;
		double FD;//��ǰ�Ƕ���ֵ�Ļ����ϵ,FD����ֵ��Ӧ�Ƕȵ���������
		float ArcStartAngle;//�����������ʼ�㣬һ��С���յ�
		float ArcEndAngle;//����������յ㣬һ��������ʼ��
		double CurrentValue;
		double CurrentMaxValue;
		double CurrentMinValue;
		double CurrentUpperWarnValue;
		double CurrentLowerWarnValue;
		double CurrentUpperDangerValue;
		double CurrentLowerDangerValue;
	};
}


