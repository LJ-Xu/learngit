#pragma once
#include "stdafx.h"
//#define HMI_PC_RENDER
namespace Project
{
	struct HMIProject;
}
namespace UI
{
	class BaseControl;
	class BaseGControl;
	class IControlFactory
	{
	public :
		static IControlFactory *Ins();
		static void  SetIns(IControlFactory * ins);
		virtual std::unique_ptr<BaseControl> GetNewControlByName(string ctrname) = 0;
		virtual std::unique_ptr<BaseGControl> GetNewGControlByID(int ctrlEm) = 0;
	private:
		static IControlFactory* ins_;
	};
	
}