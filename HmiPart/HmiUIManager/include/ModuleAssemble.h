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
		//DataService::IDataService*       DataSrv;//数据服务
		//IResourceService*				 ResSrv;//资源操作服务
		IControlFactory*                 CtrlFactory;
		IPermission*                     Perm;

	};

}