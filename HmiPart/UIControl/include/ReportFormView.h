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
		int DisplayMode = 0;				//0:正常模式 1:查询模式
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
		int fontSize_;					//列表文字大小
		int fontStyle_;					//列表字体

		Fl_Color cell_bgcolor_;					//列表背景颜色
		Fl_Color serialcolor_;					//列表序号文字颜色
		Fl_Color datecolor_;					//列表日期文字颜色
		Fl_Color timecolor_;					//列表时间文字颜色
		vector<Fl_Color> colcolor_;				//每列文字颜色
		vector<Fl_Align> colalignstyle_;		//每列文字对齐方式
		vector<int> channelno_;
		int gridstarty_ = 0;
	};
}

