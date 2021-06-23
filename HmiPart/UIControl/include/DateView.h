#pragma once
#include "stdafx.h"
#include <BasicView.h>
#include <FL/Fl_Box.H>
namespace UI
{
	class DateView : public HMIBaseView
	{
	public:
		DateView(int X, int Y, int W, int H);
		~DateView();
		BasicView BaseView;
		void InitBackPic(void* model);
		//��ȡ��ǰ����,���ظ�ʽ������ַ���
		//FormatType:���ڸ�ʽ
		//0:yyyy/MM/dd  
		//1:yyyy-MM-dd
		//2:yyyy mm dd
		//3:yyyy��MM��dd��  (����)
		//4:2020 Sep Day1   (Ӣ��)
		static std::string GetCurrentDate(int FormatType = 0,bool showFullYear=true,bool showWeek=false);
		bool HandleOperatePerm();
	protected:
		void draw() override;
	private:
		Fl_Image *flImgPtr;
		int FinX;
		int FinY;
	};
}

