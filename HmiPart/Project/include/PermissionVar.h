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
		bool GetUsePms()		//�Ƿ����û�����Ȩ��
		{
			//return UI::Services()->DataSrv->GetNumber<bool>(permData);
			return true;
		}
		bool GetDisplayPms()	//�Ƿ�����ʾȨ��
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

