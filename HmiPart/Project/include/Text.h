#pragma once
#include "HFont.h"
#include "Color.h"
#include "list"
namespace Project
{
	class Text
	{
	public:
		list<string>message;
		HFont cellfont;
		Color normalcolor;
		Color confirmcolor;
		Color recovercolor;
		int datetype;
		enum TextType
		{
			NORMAL,
			CONFIRM,
			RECOVER
		};
		TextType _texttype;
	};

}