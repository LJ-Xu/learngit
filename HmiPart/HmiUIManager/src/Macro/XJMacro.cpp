#include "DataApi.h"
#include "XJMacro.h"

namespace UI
{

	int XJMacro::Trig_Macro(char*  macroId)
	{
		DataApi::MacroTrigger(macroId);
		//XJMacro::Trig_Macro(macroId.c_str());
		return 0;
	}

		//转换表达式 ，传入输入值
	int XJMacro::Trig_Express(int eid, DDWORD& minData, XJDataType& minInfo, DDWORD& out)
	{
		//
		return 0;
	}


}
