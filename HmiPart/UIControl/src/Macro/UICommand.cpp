#include "UIData.h"
#include "UICommand.h"
#include "CodeFormatUtility.h"
namespace UI
{
	UICommand* UICommand::ins_;

	UICommand::UICommand(Project::HMIProject* prj)
	{
		prj_ = prj;
	}

	UICommand* UICommand::Ins()
	{
		return ins_;
	}

	void  UICommand::SetIns(UICommand* ins)
	{
		ins_ = ins;
	}

	bool UICommand::GetUsrList(std::string param, vector<string>& data)
	{
		if (param == "UL(用户列表)")
		{
			vector<string>().swap(data);
			if (!prj_)
				return false;
			for (size_t i = 0; i < prj_->Tables.User.Users.size(); i++)
			{
				data.push_back(prj_->Tables.User.Users[i].Uname);
			}
			return true;
		}
		return false;
	}
	bool UICommand::LogInUser(std::string param, vector<string>& data)
	{
		int namelen = LocalData::GetLocalVarLen(SYS_PSW_LOGINName);
		char *loginusername = new char[namelen + 1];
		memset(loginusername, '\0', namelen + 1);
		LocalData::GetString(SYS_PSW_LOGINName, loginusername);

		int pwdlen = LocalData::GetLocalVarLen(SYS_PSW_LOGINPwd);
		char *userpwd = new char[pwdlen + 1];
		memset(userpwd, '\0', pwdlen + 1);
		LocalData::GetString(SYS_PSW_LOGINPwd, userpwd);

		//char loginusername[32] = { 0 };
		//char userpwd[32] = { 0 };
		//LocalData::GetString(SYS_PSW_LOGINName, loginusername);
		//LocalData::GetString(SYS_PSW_LOGINPwd, userpwd);
		for (size_t i = 0; i < prj_->Tables.User.Users.size(); i++)
		{
			string username = prj_->Tables.User.Users[i].Uname;
			if (!UI::CodeFormatUtility::IsStrUtf8(username.c_str()))
				UI::IResourceService::GB2312toUtf8(username);
			if (username == loginusername
				&& prj_->Tables.User.Users[i].Passwd == userpwd)			//匹配登陆
			{
				LocalData::SetString(SYS_PSW_LoginedName, loginusername);		//设置登陆账户
				LocalData::SetBit(SYS_PSB_HASLOGINUSER, true);				//标志已有用户登陆
				LocalData::SetBit(SYS_PSB_HASLOGOUTUSER, false);				//标志已注销用户登陆
				LocalData::SetNumberData(SYS_PSW_LOGINFlag, 1);				//登陆成功
				delete[] loginusername;
				delete[] userpwd;
				return true;
			}
		}
		LocalData::SetNumberData(SYS_PSW_LOGINFlag, 2);						//设置登陆失败
		delete[] loginusername;
		delete[] userpwd;
		return false;
	}
	bool UICommand::LogOutUser(std::string param, vector<string>& data)
	{
		char username[32] = { 0 };
		char userpwd[32] = { 0 };
		char loginusername[32] = { 0 };
		LocalData::GetString(SYS_PSW_LOGINName, username);
		LocalData::GetString(SYS_PSW_LoginedName, loginusername);
		LocalData::GetString(SYS_PSW_LOGINPwd, userpwd);
		if(!strncmp(loginusername, username,sizeof(username)))
		//if (loginusername == username)
		{
			for (size_t i = 0; i < prj_->Tables.User.Users.size(); i++)
			{
				if (prj_->Tables.User.Users[i].Passwd == userpwd)
				{
					LocalData::SetString(SYS_PSW_LoginedName, "");				//设置登陆账户
					LocalData::SetBit(SYS_PSB_HASLOGINUSER, false);				//标志未有用户登陆
					LocalData::SetBit(SYS_PSB_HASLOGOUTUSER, true);				//标志注销用户登陆
					LocalData::SetNumberData(SYS_PSW_LOGINFlag, 3);				//注销成功
					/*通知部件权限有变化*/
					return true;
				}
			}
		}
		LocalData::SetNumberData(SYS_PSW_LOGINFlag, 4);							//注销失败
		return false;
	}

	int UICommand::GetUserLimitRegion(std::string username)
	{
		int region = 0;
		for (size_t i = 0; i < prj_->Tables.User.Users.size(); i++)
		{
			string name = prj_->Tables.User.Users[i].Uname;
			if (!UI::CodeFormatUtility::IsStrUtf8(name.c_str()))
				UI::IResourceService::GB2312toUtf8(name);
			if (name == username)
				return prj_->Tables.User.Users[i].Region;
		}
		return 0;
	}
	std::string UICommand::GetDeviceName(int devid)
	{
		if (prj_->Devices.Devs.size() > (size_t)devid)
			return prj_->Devices.Devs[devid].DevName;
		return "";
	}


}