#pragma once
#include "SampleInfoRes.h"
#include "ReportChannel.h"
#include "AlarmDisplayConfig.h"
#include "ReportFormUnit.h"
namespace Project
{

	class ReportFormConfig	: public ReportFormUnit
	{
	public:
		int SimpleGroupName;				//采样组
		int SimpleGroupNo;					//采样编号
		DataVarId SampleVarId;				//通知采样数据变化寄存器
		vector<ReportChannel> ReportChannelLst;		//报表通道设置
		bool IsViewSerialNo;				//是否显示序号
		int SerialNum;						//序号位数
		int SerialColor;					//序号颜色
		bool IsViewDate;					//是否显示日期
		int DateColor;						//日期颜色
		DateMode DateStyle;					//日期格式
		int IsViewTime;						//是否显示时间
		int TimeColor;						//时间颜色
		TimeMode TimeStyle;					//时间格式
		int MaxRows;						//最大行数
		int PerPageRows;					//每页行数
		int ViewSort;						//显示顺序	0：顺序 1：逆序
		bool IsModifyCellValue;				//是否可修改单元格内容
		bool IsViewTitle;					//是否显示标题
		int TitleHeight = 0;					//标题高度
		StatusRes TitleRes;					//标题文字内容
		StringStyle ElementTitleStyle;		//标题样式
		bool IsViewEachColTitle;			//是否使用列标题
		int EachColTitleHeight = 0;			//列标题高度
		bool IsUseMultiLan;					//是的使用多语言
		bool IsAutoRowColWidth;				//是否自动行高列宽
		//添加需要显示的信息
		vector<DisplayReportLst> InfoLst;	//显示信息项目
		StringStyle TitleBarStyle;			//列标题样式
		StringStyle ListFontStyle;			//列表文字样式


		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(SimpleGroupName, SimpleGroupNo, ReportChannelLst, IsViewSerialNo, SerialNum, SerialColor,
				IsViewDate, DateColor, DateStyle, IsViewTime, TimeColor, EachColTitleHeight, SampleVarId,
				TimeStyle, MaxRows, PerPageRows, ViewSort, IsModifyCellValue, IsViewTitle, TitleHeight,
				TitleRes, ElementTitleStyle, IsViewEachColTitle, IsUseMultiLan, IsAutoRowColWidth,
				InfoLst, TitleBarStyle, ListFontStyle, Appearance, Key, TableBgColor, TitleBgColor,
				IsUseBorder, TableBorderStyle, GridBorderStyle, IsRowDividLine, IsColDividLine,
				EnableSearch, SearchMode, SeekModeVar, SeekCtrlVar, SearchDate, SearchDateVarId,
				SearchTimeStart, SearchTimeEnd, SearchTimeVar, SearchGroup, SearchGroupVar,
				SearchNo, SearchNoVar, SearchLevel, RegMode, SearchRegVarId,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}
