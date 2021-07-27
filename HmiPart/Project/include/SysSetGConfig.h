#pragma once
#include "ProjectSystemSet.h"
namespace Project
{
	class SysSetGConfig
	{
	public:
		ClockSet Clock;						//ʱ������
		ExchangeSet Exchange;				//��������
		DataVarId StartWinNoVarId;			//����ҳ���
		DataVarId ScreenSaverTimeVarId;		//����ʱ��
		DataVarId BacklightTimeVarId;		//�����ʱ��

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Exchange, Clock, StartWinNoVarId, ScreenSaverTimeVarId, BacklightTimeVarId);
		}

	};

}