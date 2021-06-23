#pragma once
#include "DataTableUnit.h"
#include "ReportChannel.h"
#include "StatusRes.h"
#include "StringStyle.h"
namespace Project
{
	class DataTableConfig :public DataTableUnit
	{
	public:
		bool UseContineAddr;					//是否使用连续地址
		int RowsNum;							//行数
		int PerPageRows;						//每页行数
		int ColsNum;							//列数
		
		bool UseTitleBar;						//是否使用标题
		int TitleHeight;						//标题高
		StatusRes TitleRes;						//标题内容
		bool ColTitleUsed;						//是否使用列标题
		int ColTitleHeight;						//列标题高
		bool ColTitleMultiLangsUsed;			//列标题是否使用多语言
		vector<DisplayReportLst> ColInfoLst;	//每列内容信息
		int SerialNum;							//序号位数
		vector<StatusRes> StaticColName;		//静态列名称
		bool UseSameStyle;						//是否使用同一字体
		StringStyle TitleBarStyle;				//标题样式
		StringStyle ListFontStyle;				//列表文字样式
		StringStyle SameFontStyle;				//同样式

		vector<DataCell> DataCellInfoLst;		//表格单元格设置

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(UseContineAddr, RowsNum, PerPageRows, ColsNum, UseTitleBar, ColTitleHeight,
				ColInfoLst, SerialNum, StaticColName, UseSameStyle, TitleBarStyle, TitleRes,
				ListFontStyle, SameFontStyle, DataCellInfoLst, TitleHeight, ColTitleUsed,
				Appearance, Key, TableBgColor, TitleBgColor, IsUseBorder, ColTitleMultiLangsUsed,
				TableBorderStyle, GridBorderStyle, IsRowDividLine, IsColDividLine,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}


