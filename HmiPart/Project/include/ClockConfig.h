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
	class ClockConfig : public InputNumShowUnit
	{
	public:

		//0: HH:MM:SS
		//1: HH:MM
		int TimeFormat = 0;
		//0 12小时制
		//1 24小时制
		int HourType = 0;

		ImageResId BackUpImg;
		StringStyle ClockFontStyle;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y, Width, Height, CtrlName, VOffX, VOffY, Perm, TimeFormat, HourType, BackUpImg, ClockFontStyle);
		}
	};
}
