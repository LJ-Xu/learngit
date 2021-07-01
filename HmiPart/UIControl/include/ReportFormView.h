#pragma once
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Table_Row.H>
#include "SampleInfoRes.h"
#include "ReportFormModel.h"
#include "ReportFormControl.h"
#include "SampleStorage.h"
typedef std::chrono::milliseconds MilliSecs;

namespace UI
{
	class ReportFormView : public HMIBaseTable
	{
	public:

		ReportFormView(int X, int Y, int W, int H, const char *l);
		~ReportFormView();
		void InitDraw(void);
		BasicView BaseView;
		//int handle(int event);
		int DisplayMode = 0;				//0:����ģʽ 1:��ѯģʽ
		vector<vector<Storage::SampleRecord>> ChannelData;
		int GetSampleDataNum();
		int GetChannelNo(int no);
		void SetRenderData();
		string GetTimeString(DDWORD time, int mode);
		string GetDrawString(string projectname,int row);
	protected:
		void draw_cell(TableContext context,  		// table cell drawing
			int R = 0, int C = 0, int X = 0, int Y = 0, int W = 0, int H = 0);
	private:
		int fontSize_;					//�б����ִ�С
		int fontStyle_;					//�б�����

		Fl_Color cell_bgcolor_;					//�б�����ɫ
		Fl_Color serialcolor_;					//�б����������ɫ
		Fl_Color datecolor_;					//�б�����������ɫ
		Fl_Color timecolor_;					//�б�ʱ��������ɫ
		vector<Fl_Color> colcolor_;				//ÿ��������ɫ
		vector<Fl_Align> colalignstyle_;		//ÿ�����ֶ��뷽ʽ
		vector<int> channelno_;
		int gridstarty_ = 0;
	};
}

