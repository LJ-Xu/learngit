#pragma once
#include "Sysdef.h"
#include <string>
namespace UI
{
	class XJMacro
	{
	public:
		static int Trig_Macro(char* macroId);

		//ת�����ʽ ����������ֵ
		static int Trig_Express(int eid, DDWORD& minData, XJDataType& minInfo, DDWORD& out);//�첽ִ��
	};

}

