#pragma once
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Table_Row.H>
#include "DataTableModel.h"
#include "DataTableControl.h"
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
namespace UI
{
	enum Language {Charactinput = 0,Chineseinput = 1};
	class DataTableView : public HMIBaseTable
	{
	public:

		DataTableView(int X, int Y, int W, int H, const char *l);
		~DataTableView();
		void InitDraw(void);
		BasicView BaseView;
		int handle(int event);
		string GetDrawString(int row, int col);
		void EnterValue();
		bool HandleOperateConfirm();
		void EndInput();
		bool HandleOperatePerm();
		void InitStartInput(int mx, int my);
	protected:
		void draw_cell(TableContext context,  		// table cell drawing
			int R = 0, int C = 0, int X = 0, int Y = 0, int W = 0, int H = 0);
	private:
		int datacolnum_;						//数据列数
		int scrollsize_ = 0;
		int fontSize_;							//列表文字大小
		int fontStyle_;							//列表字体
		bool useserial_ = false;
		bool usestaticcol_ = false;
		Fl_Color cell_bgcolor_;					//列表背景颜色
		int gridstarty_ = 0;
		int gridstartrow_ = 0;
		bool haskeyboard_ = false;				//是否有键盘弹出
		bool hasPinYinpage = false;
		bool firstopen_ = true;

		Project::DataCell selectcell_;
		int selectedcol_ = -1;					//选择的列
		int selectedrow_ = -1;					//选择的行


		int clickcount_ = 0;								//记录部件按下次数
		high_resolution_clock::time_point firstTime_;		//记录第一次按键按下时间
		high_resolution_clock::time_point lastTime_;		//记录最后一次按键按下时间
		high_resolution_clock::time_point pushTime_;		//记录按键按下时间
		high_resolution_clock::time_point releaseTime_;		//记录按键松开时间
	
		string inputtext_;
		string pinyin_;
		vector<int> languageflag_;				//true：中文，false：字符
	};
}