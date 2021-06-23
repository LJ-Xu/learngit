#pragma once
#include <map>
#include <string>
#include "HMIProject.h"
using namespace std;

namespace Project
{
	struct HMIProject;
}
namespace UI
{

	class UICommand
	{
	public:
		UICommand(Project::HMIProject* prj);
		UICommand() {};
		static UICommand* Ins();
		static void  SetIns(UICommand* ins);
		bool GetUsrList(std::string param, vector<string>& data);
		int GetUserLimitRegion(std::string username);
		std::string GetDeviceName(int devid);
		bool LogInUser(std::string username, vector<string>& data);
		bool LogOutUser(std::string username, vector<string>& data);
	private:
		static UICommand* ins_;
		Project::HMIProject* prj_;
	};
	static map<string, bool(*)(string param, vector<string>& data)> GetCommandDatas = {
		{"GetUserList",[](std::string param, vector<string>& data) {if (!UICommand::Ins()) return false; return UICommand::Ins()->GetUsrList(param, data);}},
		{"LogInUser",[](std::string param, vector<string>& data) {if (!UICommand::Ins()) return false; return UICommand::Ins()->LogInUser(param, data);  }},
		{"LogOutUser",[](std::string param, vector<string>& data) {if (!UICommand::Ins()) return false; return UICommand::Ins()->LogOutUser(param, data); }},
	};
}
