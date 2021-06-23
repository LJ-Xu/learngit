#include "UIStorage.h"
#include "IPermission.h"
namespace UI
{
	void UIStorage::Alarm_Add()
	{
		Storage::AlarmRecord recd;
	}
	void UIStorage::History_Add(Storage::RecordEnum rcdEM, Project::DataVarId varid,
								Project::WinCtrlID ctrlid,DDWORD oldData)
	{		
		Storage::OperatorRecord recd;
		SysUser* usr = IPermission::Ins()->CurUser();
	}
	void UIStorage::Sample_Add()
	{
		Storage::SampleRecord recd;
	}
}