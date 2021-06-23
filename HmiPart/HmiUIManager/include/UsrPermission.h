#pragma once
#include "IPermission.h"
#include "HMIProject.h"
#include "ProjectDataTables.h"
namespace UI
{
	class UsrPermission :public IPermission
	{
	public:
		UsrPermission(Project::HMIProject* prj);
		bool HasPerm(int requestRegion) override;
		bool UserLogin(string uname, string passwd) override;
		SysUser* CurUser()  override;
	private:
		Project::HMIProject* prjData_;
		//Project::UserItem userData_;		//存放当前用户权限信息
		UI::SysUser user_;
	};

}

