#pragma once
#include "SampleInfoRes.h"
#include "ReportChannel.h"
#include "AlarmDisplayConfig.h"
namespace Project
{
	enum OperatorSeekMode	{ OperatorByDate, OperatorByTime, OperatorByName, OperatorByReg};
	class OperatorChartConfig : public AlarmSeekUnit
	{
	public:
		vector<DisplayReportLst> DisplayItems;		//需要显示的信息
		DataVarId RecordVarId;						//通知操作记录变换寄存器

		bool UseMultiLanguage;						//是否使用多语言
		int SortMode;								//0：时间顺序；1：时间逆序
		TimeMode TimeFormate;						//时间格式
		DateMode DateFormate;						//日期格式
		int RowHeight;								//行高
		bool UsedTitle;								//是否使用标题
		StatusRes TitleStringRes;					//标题
		int TitleHeight;							//标题行高

		bool IsUseSameStyle;						//是否使用相同字体
		StringStyle TitleStringStyle;				//标题文字样式
		StringStyle RecordItemStringStyle;			//列表文字样式

		int TableBgColor;							//列表背景颜色
		int TitleBgColor;							//标题背景颜色
		bool IsUseBorder;							//是否使用外框
		BaseLine TableBorderStyle;					//外框线样式
		BaseLine GridBorderStyle;					//网格线样式
		bool IsRowDividLine;						//是否使用行网格线
		bool IsColDividLine;						//是否使用列网格线

		OperatorSeekMode SearchMode;					//查询模式

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(DisplayItems, UseMultiLanguage, SortMode, TimeFormate, DateFormate, RecordVarId,
				RowHeight, UsedTitle, TitleStringRes, TitleHeight, IsUseSameStyle, TitleStringStyle,
				RecordItemStringStyle,TableBgColor, TitleBgColor, IsUseBorder, TableBorderStyle, 
				GridBorderStyle, IsRowDividLine, IsColDividLine, EnableSearch, SearchMode, SeekModeVar, 
				SeekCtrlVar, SearchDate, SearchDateVarId, SearchTimeStart, SearchTimeEnd, RegMode, SearchRegVarId,
				SearchKey, SearchKeyVar, VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}
