#pragma once
#include "BasicView.h"
#include <stdafx.h>
#include "OperatorChartModel.h"
#include "OperatorChartControl.h"
#include "OperatorStorageService.h"
typedef std::chrono::milliseconds MilliSecs;

namespace UI
{
	class OperatorChartView : public HMIBaseTable
	{
	public:
		OperatorChartView(int X, int Y, int W, int H, const char *l);
		~OperatorChartView();
		void InitDraw(void);
		BasicView BaseView;
		int DisplayMode = 0;				//0:正常模式 1:查询模式
		vector<Storage::OperatorRecord> RecordData;
		int GetRecordData();
		void SetRenderData();
		string GetTimeString(DDWORD time, int mode);
		string GetDrawString(string projectname, int row);
	protected:
		void draw_cell(TableContext context,  		// table cell drawing
			int R = 0, int C = 0, int X = 0, int Y = 0, int W = 0, int H = 0);
	private:
		int fontSize_;							//列表文字大小
		int fontStyle_;							//列表字体

		Fl_Color cell_bgcolor_;					//列表背景颜色
		int gridstarty_ = 0;
		int gridstartx_ = 0;
		int gridstartcol_ = 0;
	};
}



