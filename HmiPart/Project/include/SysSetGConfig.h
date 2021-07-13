#pragma once
#include "ProjectSystemSet.h"
namespace Project
{
	class SysSetGConfig
	{
	public:
		ClockSet Clock;				//ʱ������
		ExchangeSet Exchange;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Exchange, Clock);
		}

	};

}