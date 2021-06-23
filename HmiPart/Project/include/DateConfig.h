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
		//���ڸ�ʽ
		//0:yy/MM/dd  
		//1:yy-MM-dd
		//2:yy mm dd
		//3:yy��MM��dd��  (����)
		//4:2020 Sep 1St   (Ӣ��)
		int DateFormat;
		//��ʾ����4λ���
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
