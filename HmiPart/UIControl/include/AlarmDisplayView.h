#pragma once
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Table_Row.H>
#include "AlarmInfoRes.h"
#include "AlarmDisplayModel.h"
#include <time.h>
#include "AlarmRecord.h"
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace UI
{

	class AlarmDisplayView : public HMIBaseTable
	{
	public:
		AlarmDisplayView(int X, int Y, int W, int H, const char *l);
		~AlarmDisplayView();
		BasicView BaseView;
		Project::AlarmDisMode DisplayMode = Project::AlarmDisMode::REALTIME;		//显示模式 0：实时	1：历史 2：查询
		int HideFlag;									//隐藏控制
		vector<Storage::AlarmRecord> DisplayInfo;		//用来显示的信息

		void InitDraw(void);							//初始化报警显示绘制的参数
		int handle(int event);							//处理报警确认
		int GetInfoNum();								//获取显示信息和显示信息长度
		string GetTimeString(DDWORD time, int mode);	//将时间转成字符串
		string GetDrawString(string projectname, int row);			//获取要绘制的字符串
		void SortAlarmInfo();							//获取排序后的显示信息（置顶未恢复信息）

		void SetRenderData();
	protected:
		void draw_cell(TableContext context,  		// table cell drawing
			int R = 0, int C = 0, int X = 0, int Y = 0, int W = 0, int H = 0);
	private:
		int fontSize_;					//列表文字大小
		int fontStyle_;					//列表字体
		Fl_Color fontColor_;			//列表文字颜色
		int fontAlign_;					//列表文字对齐方式
		int gridstarty_ = 0;
		int gridstartx_ = 0;
		int gridstartcol_ = 0;

		Fl_Color cell_bgcolor_;			//列表背景颜色
		Fl_Color triggerTextColor_;		//报警触发时文字颜色
		Fl_Color triggerbgColor_;		//报警触发时背景颜色
		Fl_Color confirmTextColor_;		//报警确认时文字颜色
		Fl_Color confirmbgColor_;		//报警确认时背景颜色
		Fl_Color recoverTextColor_;		//报警恢复时文字颜色
		Fl_Color recoverbgColor_;		//报警恢复时背景颜色

		int clickcount_ = 0;								//记录部件按下次数
		high_resolution_clock::time_point firstTime_;		//记录第一次按键按下时间
		high_resolution_clock::time_point lastTime_;		//记录最后一次按键按下时间
		high_resolution_clock::time_point pushTime_;		//记录按键按下时间
		high_resolution_clock::time_point releaseTime_;		//记录按键松开时间

	};

}