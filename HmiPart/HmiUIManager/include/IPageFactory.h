#pragma once
#include "stdafx.h"
#include "HMIPage.h"
//#define HMI_PC_RENDER
namespace Project
{
	struct HMIProject;
}
namespace UI
{
 
	class IPageFactory
	{
	public :
		static IPageFactory *Ins();
		static void  SetIns(IPageFactory * ins);
		virtual std::unique_ptr<UI::HMIPage> NewPage(short catogray, int pageid, int x, int y, int w, int h, const char* title=0) = 0;
	private:
		static IPageFactory* ins_;
	};
	
}