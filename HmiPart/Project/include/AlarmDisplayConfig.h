#pragma once
#include "ControlModel.h"
#include "StatusRes.h"
#include "StringStyle.h"
#include "AlarmTableUnit.h"
namespace Project
{
	enum ConfirmMode :char {Single, Double, Long};
	enum AlarmDisMode :char {REALTIME,HISTORY,SEARCH};		//实时、历史
	struct ColInfo
	{
		std::string ProjectName;			//显示项目
		std::string DescribeName;			//项目表述
		StatusRes TitleDescribe;			//多语言项目描述
		int RowHeight;						//行高
		int ColWidth;						//列宽
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ProjectName, DescribeName, TitleDescribe, RowHeight, ColWidth);
		}
	};
	enum TimeMode :char { HMS, HM, HMSMS,HMSChinese};
	enum DateMode :char { YMDSlash, DMYSlash, MDYSlash, YMDChinese };
	class AlarmDisplayConfig : public AlarmTableUnit
	{
	public:
		int StartGroupName;						//显示组别起始
		int EndGroupName;						//显示组别结束

		bool UseTitle;							//是否使用标题
		bool UseSameStyle = false;				//标题列表是否使用同样式
		StatusRes Title;						//标题文字
		StringStyle TitleStringStyle;			//标题字体
		StringStyle ListTitleStyle;				//列标题字体
		StringStyle ListStringStyle;			//列表字体
		StringStyle SameStringStyle;			//相同字体

		AlarmDisMode AlarmMode;					//报警模式	0：实时	1：历史 2：查询
		int AlarmMaxNum;						//报警总条数
		int PageNum;							//报警每页条数
		bool IsUseMultiLanguage;				//是否使用多语言
		vector<ColInfo> AlarmOptions;			//报警显示选项
		int TitleHeight;						//标题高度
		int EachColTitleHeight;					//列标题高度
		int SortMode;							//排序方式	0：时间顺序 1：时间逆序
		TimeMode Timemode;						//时间格式
		DateMode Datemode;						//日期格式
		bool TopNotRecover;						//是否置顶未恢复信息 0：不置顶 1：置顶
		bool UseConfirmMode;					//是否启用确认	0：不启用	1：启用
		ConfirmMode CMode;						//确认模式
		bool IsInfoHideCtrl;					//是否控制隐藏
		DataVarId HideVar;						//控制隐藏信息的寄存器
													//bit0 隐藏已确认信息
													//bit1 隐藏已恢复信息
		DataVarId AlarmVarId;					//报警信息更改
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(StartGroupName, EndGroupName, UseTitle, UseSameStyle, Title, EachColTitleHeight,
				TitleStringStyle, ListTitleStyle, ListStringStyle, SameStringStyle,
				AlarmMode, AlarmMaxNum, PageNum, IsUseMultiLanguage, AlarmOptions, TitleHeight,
				SortMode, Timemode, Datemode, TopNotRecover, UseConfirmMode, CMode, HideVar,
				BackColor, TitleBgColor, UseFrame, FrameStyle, UseGrid, GridStyle, AlarmVarId,
				AlarmTextColor, AlarmBackColor, UseConfirm, ConfirmTextColor, ConfirmBackColor,
				UseRecover, RecoverTextColor, RecoverBackColor, SearchMode, IsInfoHideCtrl,
				EnableSearch, SeekModeVar, SeekCtrlVar, SearchDate,  SearchDateVarId,
				SearchTimeStart, SearchTimeEnd, SearchTimeVar, SearchGroup, SearchGroupVar,
				SearchNo, SearchNoVar, SearchLevel, SearchLevelVar, RegMode, SearchRegVarId,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
		//int HideFlag;
		//ControlModel model;

	};
}