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
		//��ȡ��ǰʱ��,���ظ�ʽ������ַ���
		//FormatType:ʱ�侫��
		//0: HH:MM:SS
		//1: HH:MM
		//HourType:Сʱ������
		//0 12Сʱ��
		//1 24Сʱ��
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
