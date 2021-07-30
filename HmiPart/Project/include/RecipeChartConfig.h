#pragma once
#include "MoveableDisplayUnit.h"
#include "RecipeInfoRes.h"
#include "StatusRes.h"
#include "StringStyle.h"
#include "ReportChannel.h"
#include "RecipeFormUnit.h"
#include "InputNumShowUnit.h"
namespace Project
{
	class RecipeChartConfig : public RecipeFormUnit
	{
	public:
		DataVarId UpdateNotice;							//配方数据更新通知
		string RecipeGroupId;							//配方组名称
		int ArrangMode;									//排序方式 0:顺序 1:逆序
		int TotalRowCount;								//最大行数
		int PerPageRowCount;							//每页行数
		bool TitleUsed;									//是否显示标题
		int TitleHeight = 0;							//标题高度
		StatusRes TxtTitle;								//标题文字
		StringStyle TitleStyle;							//标题样式
		int EachColTitleHeight = 0;						//列标题高度
		bool ColTitleShown;								//是否使用列标题
		StringStyle ColTitleStyle;						//列标题样式
		vector<DisplayReportLst> InfoLst;				//显示信息项目
		bool SerialNoShown;								//是否显示序号
		int SerialNumStyle;								//序号样式 0：数字 1：中文
		bool IsUseMultiLan;								//是否使用多语言
		bool AutoSize;									//是否自动行高列宽
		StringStyle ListFontStyle;						//列表文字样式
		KeyboardRes KeyboardNum;						//数字键盘输入
		KeyboardRes KeyboardChar;						//字符键盘输入

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(UpdateNotice, RecipeGroupId, ArrangMode, TotalRowCount, PerPageRowCount, TitleUsed,
				TitleHeight, TxtTitle, TitleStyle, EachColTitleHeight, ColTitleShown,
				ColTitleStyle, InfoLst, SerialNoShown, SerialNumStyle, IsUseMultiLan,
				AutoSize, ListFontStyle, KeyboardNum, KeyboardChar,AppearMode, Key, TableBgColor, TitleBackColor, ColTitleBackColor,
				ParityDiffColor, OddBackColor, EvenBackColor, IsUseBorder, TableBorderStyle,
				GridBorderStyle, IsRowDividLine, IsColDividLine, FocusFontColor, FocusCellColor,
				FocusRowColor, SearchMode, EnableSearch, SeekModeVar, SeekCtrlVar, SearchDate, SearchDateVarId,
				SearchTimeStart, SearchTimeEnd, SearchTimeVar, SearchGroup, SearchGroupVar, RegMode, SearchRegVarId,
				SearchNo, SearchNoVar, SearchLevel, SearchLevelVar, SearchChannelNo, SearchChannelNoVar, SearchKey, SearchKeyVar,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

