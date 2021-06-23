#pragma once
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Table_Row.H>
#include "RecipeChartModel.h"
#include "RecipeChartControl.h"
#include "DataFormate.h"
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
namespace UI
{
	class RecipeChartView : public HMIBaseTable
	{
	public:
		RecipeChartView(int X, int Y, int W, int H, const char *l);
		~RecipeChartView();
		void InitDraw(void);
		void GetSearchData();
		string GetDrawString(string projectname, int row);
		bool HandleOperatePerm();

		BasicView BaseView;
		int DisplayMode = 0;					//0:正常模式 1:查询模式
		vector<int> SearchRows;
		//string SearchKey = "";					//查询数据
		//double SearchData;
		vector<vector<string>> RecipeDatas;		//配方表数据
	protected:
		void draw_cell(TableContext context,  		// table cell drawing
			int R = 0, int C = 0, int X = 0, int Y = 0, int W = 0, int H = 0);
		int handle(int event);
	private:
		int fontSize_;							//列表文字大小
		int fontStyle_;							//列表字体
		Fl_Color fontcolor_;
		Fl_Color cell_bgcolor_;					//列表背景颜色
		Fl_Color focuscellcolor_;				//列表选择背景颜色
		Fl_Color focusfontcolor_;				//列表选择文字颜色
		Fl_Color focusrowcolor_;				//列表选择行颜色
		Fl_Color oddbgcolor_;
		Fl_Color evenbgcolor_;
		int datacol_ = 0;
		int clickcount_ = 0;								//记录部件按下次数
		high_resolution_clock::time_point firstTime_;		//记录第一次按键按下时间
		high_resolution_clock::time_point lastTime_;		//记录最后一次按键按下时间
		high_resolution_clock::time_point pushTime_;		//记录按键按下时间
		high_resolution_clock::time_point releaseTime_;		//记录按键松开时间

		//Project::VarDataType selecteddatatype_;
		//Project::VarNumberType selecteddatafmt_;
		//Utility::NumberFmtInfo selectedfmt_;
		Project::RecipeInfoRes *selectresinfo_;
		int selectedcol_ = -1;					//选择的列
		int selectedrow_ = -1;					//选择的行
		int gridstarty_ = 0;

		bool haskeyboard_ = false;				//是否有键盘弹出
		bool firstopen_ = true;			
		string inputtext_;
		bool hasPinYinpage = false;
		string pinyin_;
	};
}
