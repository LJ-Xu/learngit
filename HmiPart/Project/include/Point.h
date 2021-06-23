/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : Point.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ×ø±êµãÀà
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
namespace Project
{
	class Point
	{
	public:
		int X;
		int Y;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y);
		}

	};
	class DoublePoint
	{
	public:
		double X;
		double Y;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y);
		}

	};
}

