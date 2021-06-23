#pragma once
#include <HFont.h>
#include <HImage.h>
#include "MoveableDisplayUnit.h"
namespace Project
{
	class GLabelConfig : public MoveableDisplayUnit
	{
	public:
		string text;
		HFont font;
		HImage image;
		int backColor;
		int type;

	};
}
