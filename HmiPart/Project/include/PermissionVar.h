#pragma once
#include "BaseVar.h"
#include "DataVar.h"
namespace Project
{
	class PermissionVar 
	{
	public:
		bool UsePms;
		int PmsValue;
		DataVarId PermData;
		bool GetUsePms()		//是否有用户更改权限
		{
			//return UI::Services()->DataSrv->GetNumber<bool>(permData);
			return true;
		}
		bool GetDisplayPms()	//是否有显示权限
		{
			//return UI::Services()->DataSrv->GetNumber<bool>(permData);
			return false;
		}

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(UsePms, PmsValue, PermData);
		}
	};

}

