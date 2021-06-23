#pragma once
#include "stdafx.h"
#include "DeleteHelper.h"
#include "UIManager.h"
#include "HMIProject.h"
#include "RunEnv.h"
namespace Core
{
	class HmiCore
	{
	public:
		int Run();
		int Init();
		int TestRun(Project::HMIProject* prj);
	private:
		int InitProject(const char* path);
		bool InitService();
		int RunModule();
		void PickUp();//退出前收拾要处理的内容
		void InitManager();
		void InitThreadService();
		void InitBasicService();
		void ExitThreadService();
		//工程数据
		Project::HMIProject* prjData_;
		UI::UIManager* uiManager_;
		DeleteHelper deleteHelper_;
 
	};
}