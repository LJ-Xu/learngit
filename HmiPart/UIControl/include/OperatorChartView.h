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
		int DisplayMode = 0;				//0:����ģʽ 1:��ѯģʽ
		vector<Storage::OperatorRecord> RecordData;
		int GetRecordData();
		void SetRenderData();
		string GetTimeString(DDWORD time, int mode);
		string GetDrawString(string projectname, int row);
	protected:
		void draw_cell(TableContext context,  		// table cell drawing
			int R = 0, int C = 0, int X = 0, int Y = 0, int W = 0, int H = 0);
	private:
		int fontSize_;							//�б����ִ�С
		int fontStyle_;							//�б�����

		Fl_Color cell_bgcolor_;					//�б�����ɫ
		int gridstarty_ = 0;
		int gridstartx_ = 0;
		int gridstartcol_ = 0;
	};
}



