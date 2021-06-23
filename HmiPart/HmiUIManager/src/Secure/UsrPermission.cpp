#include "UsrPermission.h"
namespace UI
{
	static bool HasUser(string& name)
	{
		return !name.empty();
	}
	static void ClearUser(string& name)
	{
		name.clear();
	}
	UsrPermission::UsrPermission(Project::HMIProject* prj)
	{
		prjData_ = prj;
	}
	bool UsrPermission::HasPerm(int requestRegion)
	{
		if (requestRegion&&!HasUser(user_.Name))
			return false;
		if (requestRegion & user_.Region)
			return true;
		else
			return false;
	}
	bool UsrPermission::UserLogin(string uname, string passwd)
	{
		int i;
		if (uname.empty() || passwd.empty())
			return false;
		if (prjData_->Tables.User.Users.size() == 0)
			return false;

		/*遍历用户权限表格，获取当前用户权限*/
		vector<Project::UserItem>::iterator user;
		for (i = 0, user = prjData_->Tables.User.Users.begin(); user != prjData_->Tables.User.Users.end(); user++, i++)
		{
			if ((*user).Uname == uname && (*user).Passwd == passwd)
			{
				//userData_ = (*user);
				user_.Name = user->Uname;
				user_.Region = user->Region;
				return true;
			}
		}
		return false;
	}

	SysUser* UsrPermission::CurUser()
	{
		if (HasUser(user_.Name))
			return nullptr;
		return &user_;
	}

}
