#pragma once
#include "ImageResId.h"
#include "BaseLine.h"
#include "AlarmSeekUnit.h"
#ifdef __linux
#ifdef None
#undef None
#endif
#endif
namespace Project
{
	enum  RecipeSeekMode { RecipeKey, RecipeData, RecipeREG };
	enum GridStyle
	{
		None = 0,
		Horizontal = 1,
		Vertical = 2,
		Inner = 3,
		Outer = 4,
		OuterHorizontal = 5,
		OuterVertical = 6,
		GridAll = 7
	};
	class RecipeFormUnit : public AlarmSeekUnit
	{
	public:  
		int AppearMode;				//外观样式 0：使用图片样式 1：使用自定义外观
		ImageResId Key;				//外观图片

		int TitleBackColor;			//标题背景颜色
		int ColTitleBackColor;		//列标题背景颜色
		int TableBgColor;			//列表背景颜色
		bool ParityDiffColor;		//奇偶行不同颜色
		int OddBackColor;			//奇行颜色
		int EvenBackColor;			//偶行颜色

		//GridStyle PresetBorder;		//边框设置

		bool IsUseBorder;			//是否使用外框
		BaseLine TableBorderStyle;	//外框线样式
		BaseLine GridBorderStyle;	//网格线样式
		bool IsRowDividLine;		//是否使用行网格线
		bool IsColDividLine;		//是否使用列网格线

		int FocusFontColor;
		int FocusRowColor;
		int FocusCellColor;
		RecipeSeekMode SearchMode;	//查询模式

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(AppearMode, Key, TableBgColor, TitleBackColor, ColTitleBackColor,
				ParityDiffColor, OddBackColor, EvenBackColor, IsUseBorder, TableBorderStyle,
				GridBorderStyle, IsRowDividLine, IsColDividLine, FocusFontColor, FocusCellColor,
				FocusRowColor, SearchMode);
		}

	};
}

