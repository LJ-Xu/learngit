#pragma once
#include "Sysdef.h"
#include <string>
namespace UI
{
	class XJMacro
	{
	public:
		static int Trig_Macro(char* macroId);

		//转换表达式 ，传入输入值
		static int Trig_Express(int eid, DDWORD& minData, XJDataType& minInfo, DDWORD& out);//异步执行
	};

}

