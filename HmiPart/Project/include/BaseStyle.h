#pragma once
#include "HFont.h"
#include "Color.h"

namespace Project
{
	class BaseStyle
	{
	public:
		HFont Font;
		class Color RGBColor;
		int Colors;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Font, RGBColor, Colors);
		}
	};


}
