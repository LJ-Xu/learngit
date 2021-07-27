#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChartView.h
 * Author   : qiaodan
 * Date     : 2020-11-16
 * Descript : ��������ͼ�Ļ���ͷ�ļ�����������ͼ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Scrollbar.H>
#include "SampleInfoRes.h"
#include "TrendChartConfig.h"
#include "Point.h"
#include "SampleStorage.h"
typedef std::chrono::milliseconds MilliSecs;

namespace UI
{
	void TrendChartScrollCb(Fl_Widget*, void*);	// hscrollbar callback
	class TrendChartView : public HMIBaseGroup
	{
	public:
		TrendChartView(int X, int Y, int W, int H, const char* l);
		~TrendChartView();
		int DisplayMode;				//0:����ģʽ 1:��ѯģʽ
		int Xaxislength;				//X��(������)����
		int ScrollClick = 0;				//�������Ƿ�����־
		bool UseInfoDisplay;

		Fl_Scrollbar* Hscrollbar;		//������

		/*���ͨ������*/
		vector<Storage::SampleRecord> ChannelData;
		//vector<vector<Project::SampleChannel::SampleContent>> SampleData;
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
		void DrawRenderXaxis();			/*����״̬��x������*/
		/*����ÿ������ģʽ��x��ʱ���ע*/
		void DrawXScaleDot(int dot, DDWORD time);
		/*����Y�ᡢY��̶ȼ�Y���ע*/
		void DrawYaxis();
		/*����Y���ע*/
		void DrawYaxisScale();
		void DrawEveryYaxisScale(DOUBLE max, DOUBLE min, vector<Project::ScaleInfo>& info);
		/*����դ��*/
		void DrawGrid();
		int GetChannelNo(int no);
		/*��������ͼ*/
		void DrawChannelLine(Project::TrendLineType type, double x, double y);
		void DrawChannelTrend(Project::TrendChannel channelinfo);
		void DrawSearchTrend(Project::TrendChannel channelinfo);
		/*������ʾ��Ϣ*/
		void DrawInfoDisplay();


		/*ȷ����Ϣ��ʾ�ߴ���λ��*/
		void SetDisPlayPos(int num,int dx);
		
		/*******************************************************************************
		 * Name     : GetLimitValue
		 * Descript : ��ȡ����ʱ��ֵ
		 * Input    : channelNo - ͨ�����
		 *			  max - ���ֵ
		 *			  min - ��Сֵ
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void GetLimitValue(int channelNo, DOUBLE &max, DOUBLE &min);
		/*ת��Y���ע��ʽ*/
		string ChangeDisplayFormat(DOUBLE value);
		/*******************************************************************************
		 * Name     : GetTimeString
		 * Descript : ��ȡX���עת�����ַ���
		 * Input    : time - ��Ҫת����ʱ��
		 *			  mode - 1:ʱ��	0:����
		 *			  style - ����/ʱ��ĸ�ʽ
		 * Output   : string - ʱ�䰴��ʽת������ַ���
		 * Note	    :
		 *******************************************************************************/
		string GetTimeString(DDWORD time, int mode, int style);
		/*��ȡ����Y��ƫ��ֵ*/
		int GetYCoordinate(DDWORD& data, XJDataType& info, DOUBLE max, DOUBLE min);
		/*���÷�ҳ*/
		void SetCurrentPage(int pagenum);
		/*��������*/
		void SetZoomParam(int zoom);

		void DrawRenderTrend();
		int handle(int event);
		BasicView BaseView;
	protected:
		void draw() override;
	private:
		/*����ʹ�ó���*/
		Fl_Color trendBgColor_;		//����ͼ������ɫ
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
		int perScreendot_;			//ÿ������
		int perScreenPeriod_;	    //ÿ��ʱ���
		int yaxislength;			//Y�᳤
		Project::Point origin;		//ԭ������
		int scrollheight_;			//�������߶�

		vector<Storage::SampleRecord> currentContent_;
		vector<string> title_;			//��������
		Fl_Color infoBgColor_;			//��Ϣ���ڱ�����ɫ
		Fl_Color infofontcolor_;		//��Ϣ����������ɫ
		Fl_Color infolinecolor_;		//��Ϣ��������ɫ
		int displaylinex_;				//��Ϣ��x������
		int displayNo_ = 0;				//��Ϣ�����ڵ�
		int linex_;

		int tableX_;
		int tableY_;
		int tableW_;
		int tableH_;
		int rows_;						//����
		int rowheight_;
		int columns_;					//����
		vector<int> colwidth_;


		int scalecurrentnum_;			//���������ʱ��������
		DDWORD scalecurrenttime_;
		int currentnum_;				//��ǰ��������
		DDWORD firstStartTime_;
		DDWORD firstEndTime_;
		DDWORD startTime_ = 0;		//���������¿�ʼʱ��
		DDWORD clickstarttime_ = 0;	//�������ʾʱ��
		bool hasfirstStartTime_ = false;

		bool hasxscaledot = false;
	};
}


