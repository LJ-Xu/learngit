#pragma once
#include "MoveableDisplayUnit.h"
#include "HFont.h"
namespace Project
{
	class ChineseBtnConfig : public MoveableDisplayUnit
	{
	public:
		string text;
		HFont font;
		int backColor;
		int AcceptTime = 0;
	};
}
