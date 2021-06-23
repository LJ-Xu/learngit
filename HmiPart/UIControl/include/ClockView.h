#pragma once
#include "stdafx.h"
#include "BasicView.h"
namespace UI
{
	class ClockView :
		public HMIBaseView
	{
	public:
		ClockView(int X, int Y, int W, int H);
		~ClockView();
		BasicView BaseView;
		void InitBackPic(void* model);
		//获取当前时间,返回格式化后的字符串
		//FormatType:时间精度
		//0: HH:MM:SS
		//1: HH:MM
		//HourType:小时制类型
		//0 12小时制
		//1 24小时制
		std::string GetCurrentClock(int FormatType = 0, int HourType = 0);
		bool HandleOperatePerm();

	protected:
		void draw() override;
	private:
		Fl_Image *flImgPtr;
		int FinX;
		int FinY;
		//wstring* wscnBuff;
/*
		std::string cnstrH;
		std::string cnstrM;
		std::string cnstrS;*/
	};
}
