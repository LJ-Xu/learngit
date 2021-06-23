#pragma once
#include "ProjectSystemSet.h"
namespace Project
{
	class SysSetGConfig
	{
	public:
		ExchangeSet Exchange;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Exchange);
		}

	};

}