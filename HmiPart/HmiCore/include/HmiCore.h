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
		void PickUp();//�˳�ǰ��ʰҪ���������
		void InitManager();
		void InitThreadService();
		void InitBasicService();
		void ExitThreadService();
		//��������
		Project::HMIProject* prjData_;
		UI::UIManager* uiManager_;
		DeleteHelper deleteHelper_;
 
	};
}