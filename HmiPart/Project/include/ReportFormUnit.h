#pragma once
#include "ImageResId.h"
#include "BaseLine.h"
#include "AlarmSeekUnit.h"
namespace Project
{
	enum  ReportSeekMode { ReportDATE, ReportTIME, ReportREG };
	class ReportFormUnit : public AlarmSeekUnit
	{
	public:
		int Appearance;				//外观样式 0：使用图片样式 1：使用自定义外观
		ImageResId Key;				//外观图片

		int TableBgColor;			//列表背景颜色
		int TitleBgColor;			//标题背景颜色
		bool IsUseBorder;			//是否使用外框
		BaseLine TableBorderStyle;	//外框线样式
		BaseLine GridBorderStyle;	//网格线样式
		bool IsRowDividLine;		//是否使用行网格线
		bool IsColDividLine;		//是否使用列网格线
		ReportSeekMode SearchMode;	//查询模式
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Appearance, Key, TableBgColor, TitleBgColor, IsUseBorder, TableBorderStyle,
				GridBorderStyle, IsRowDividLine, IsColDividLine, SearchMode);
		}
	};

}

