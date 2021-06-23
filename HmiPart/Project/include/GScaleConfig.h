#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"


namespace Project
{


	class GScaleConfig : public MoveableDisplayUnit
	{
	public:

		int Scale_style;          //刻度样式

		int Maj_scale;            //主刻度数
		int Second_scale;         //次刻度数

		int Scale_length;         //刻度长度

		bool isText;              //是否标注文字
		int Start_Seg;			  //起始段
		int End_Seg;		      //结束段
		int Font_Size;            //字体大小

		enum { Up_Arc, Down_Arc, Circle, Three_Four, Horizontal, Vertical };

		BaseLine Scale_line;      //线  颜色 样式 粗细 透明度
	};



}