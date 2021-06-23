#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
namespace Project
{
	enum FuncType
	{OpenDialog,CloseDialog,TimerF,StringF,AddValueF,SubValueF};
	class GTestConfig : public MoveableDisplayUnit
	{
	public:
		int ID;
		int FuncID;
		DataVarId Val;
		string BGKey;
		string Title;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ID, FuncID,BGKey, Title, Val,X,Y,Width,Height,VOffX,CtrlName);
		}
	};

}