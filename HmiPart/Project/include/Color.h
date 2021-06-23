/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : Color.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once

namespace Project
{
	class Color
	{
	public:
		//int get_color() { return fl_color;}
		unsigned char A;
		unsigned char R;
		unsigned char G;
		unsigned char B;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(R, G, B, A);
		}

		Color() {}
		Color(unsigned char r, unsigned char g, unsigned char b) {
			this->R = r;
			this->G = g;
			this->B = b;
		}
	};
}
