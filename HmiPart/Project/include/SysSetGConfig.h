#pragma once
#include "ProjectSystemSet.h"
namespace Project
{
	class SysSetGConfig
	{
	public:
		ClockSet Clock;						//时钟设置
		ExchangeSet Exchange;				//交互设置
		DataVarId StartWinNoVarId;			//启动页面号
		DataVarId ScreenSaverTimeVarId;		//屏保时间
		DataVarId BacklightTimeVarId;		//背光灯时间

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Exchange, Clock, StartWinNoVarId, ScreenSaverTimeVarId, BacklightTimeVarId);
		}

	};

}