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
		//���������У������ض������н����ڵ���������ľ�������ptSΪ���Ͻǣ�ptEΪ���½�
		//   ---------
		//	 |   |   |
		//   |   |   |
		//   | 1 | 0 |
		//   ---------
		//	 | 2 | 3 |
		//   |   |   |
		//   |   |   |
		//   ---------
		//�������޵İ�������ڱ�ʶ
		void getarcrect(double startAngle, double endAngle, int& containquardences, int& existquardence);
		//����������ھ�������������
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
		//������ʽ
		void sectorstyle(GStickChartModel* model);
		//ֱ����ʽ
		void straightstyle(GStickChartModel* model);
		void showpecent(GStickChartModel* model);
		//ֱ����ͼ
		void getstraightfadeimgaera(Fl_Image* img,int fx,int fy,int fw,int fh);
		//full_Ϊ������������ͳ���b_Ϊ������ݵĵ�ǰ����ͳ���
		void calcustarightfillinfield(int direction, int fullx,int fully,int fullw,int fullh, bool useImg,int& bx, int& by, int& bw, int& bh);
		//���λ�ͼ
		void getsectorfadeimgaera(Fl_Image* img,int radius,double angleS,double angleE);
	private:
		char GradFormat[20];//��ǰ�̶ȸ�ʽ ����06.2f = 123.10
		char ChPecent[5];//��ǰ�̶Ȱٷֱ�
		Fl_Image* PtrBarImg;//�����ͼ�ı��������䡢���Ƶȣ�
		Fl_Image* PtrUDBarImg;
		int BarX;
		int BarY;
		int FinX;
		int FinY;
	public:
		int CurValue_;//��ǰ��ֵ
		int CurMaxValue_; //��ǰ���ֵ
		int CurMinValue_; //��ǰ��Сֵ
		int CurDstValue_; //��ǰĿ��ֵ
		int CurRangeValue_;//��ǰ��Χ
		int CurUpperValue_;//��ǰ�Ͼ���ֵ
		int CurLowerValue_;//��ǰ�¾���ֵ
		int CurBarColor_; //��ǰ��ɫ
		bool IsSpecialValue_;//��ǰ�Ƿ�Ϊ����ֵ������Σ���������¾�����Ŀ�����䣩
	};
}