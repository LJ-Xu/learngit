#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : XYLineChartView.h
 * Author   : qiaodan
 * Date     : 2020-17-08
 * Descript : ����XY����ͼ�Ļ���ͷ�ļ���������ʾ������֡�����ͼƬ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Button.H>
#include "SampleInfoRes.h"
#include "XYLineChartConfig.h"
#include "Point.h"
namespace UI
{
	void XYLineScrollCb(Fl_Widget*, void*);	// hscrollbar callback
	class XYLineChartView : public HMIBaseGroup
	{
	public:
		XYLineChartView(int X, int Y, int W, int H, const char* l);
		~XYLineChartView();

		int Xaxislength;					//X��(������)����
		int ScrollClick = 0;				//�������Ƿ�����־
		int ScrollValue;					//����ֵ
		Fl_Scrollbar* Hscrollbar;			//������

		int SampleNum;						//��������
		int PageNo = 1;
		/*���ͨ������*/
		vector<vector<DDWORD>> XData;		//X��������
		vector<vector<DDWORD>> YData;		//Y��������
		/*******************************************************************************
		 * Name     : InitDraw
		 * Descript : ��ʼ������ʹ�õ�һЩ����
		 * Input    :
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void InitDraw();
		/*����X�ᡢX��̶ȼ�X���ע*/
		void DrawXaxis();
		/*����Y�ᡢY��̶ȼ�Y���ע*/
		void DrawYaxis();
		/*����դ��*/
		void DrawGrid();
		/*����XY����ͼ*/
		void DrawChannelXYLine(Project::XYLineChannel channelinfo, int startnum, int num);
		/*���Ƶ�*/
		void DrawDot(Project::DotStyle style, int x, int y, int size);
		/*���Ʋο���*/
		void DrawReferenceLine();
		/*ת����ע��ʽ*/
		string ChangeDisplayFormat(float value,int integer,int decimal);

		void DrawRenderChart();
		//int handle(int event);
		BasicView BaseView;
	protected:
		void draw() override;
	private:
		/*����ʹ�ó���*/
		Fl_Color chartBgColor_;		//����ͼͼ������ɫ
		Fl_Color scaleBgColor_;		//�̶���������ɫ
		bool useGrid;				//�Ƿ�ʹ��դ��
		bool useXAxis;				//�Ƿ�ʹ��X��
		int xscalefontSize_;		//X���ע�����С
		int xscalefontStyle_;		//X���ע�����ʽ
		Fl_Color xscalefontColor_;	//X���ע������ɫ
		int xscaleInterval_;		//x���ע��϶
		int xscaleNum = 0;			//X�ᵱǰ��ע��

		int yscalefontSize_;		//Y���ע�����С
		int yscalefontStyle_;		//Y���ע�����ʽ
		Fl_Color yscalefontColor_;	//Y���ע������ɫ

		bool useYAxis;				//�Ƿ�ʹ��Y��
		int yaxislength;			//Y�᳤
		Project::Point origin;		//ԭ������
		int scrollheight_;			//�������߶�

		Fl_Color currentdotColor_;		//�̶���������ɫ

		int perScreendot_;			//ÿ������
		int startnum_;

	};
}
