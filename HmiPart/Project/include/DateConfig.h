#pragma once
#include "MoveableDisplayUnit.h"
#include "InputNumShowUnit.h"
#include "MacroRes.h"
#include "Color.h"
#include <HImage.h>
#include "ImageResId.h"
#include "StringStyle.h"
namespace Project
{
	class DateConfig : public InputNumShowUnit
	{
	public:
		//日期格式
		//0:yy/MM/dd  
		//1:yy-MM-dd
		//2:yy mm dd
		//3:yy年MM月dd日  (中文)
		//4:2020 Sep 1St   (英文)
		int DateFormat;
		//显示完整4位年份
		bool ShowFullYear;
		bool ShowWeek;

		ImageResId BackUpImg;
		StringStyle DateFontStyle;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y, VOffX, VOffY, Perm, Width, Height, CtrlName, DateFormat, ShowFullYear, ShowWeek, BackUpImg, DateFontStyle);
		}
	};
}
