#pragma once
#include "IResourceService.h"
#include "IDataService.h"
#include "IControlFactory.h"
#include "IPermission.h"
namespace DataService
{
	class IDataService;
} 
namespace UI
{
	class IResourceService;
 
	struct ModuleService
	{
		//DataService::IDataService*       DataSrv;//���ݷ���
		//IResourceService*				 ResSrv;//��Դ��������
		IControlFactory*                 CtrlFactory;
		IPermission*                     Perm;

	};

}