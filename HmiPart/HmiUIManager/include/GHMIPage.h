#pragma once
#include <vector>
#include "HMIPage.h"
#include "BaseGControl.h"
namespace UI
{
	class GHMIPage : public HMIPage
	{
	public:
		GHMIPage(int pageid, int x, int y, int w, int h, const char* title = 0);
		~GHMIPage();
		void draw() {};
		//不在UI线程执行的函数
		void HandleDataNotify(int ctrlidx, Project::DataVarId id);
	private:
		//BaseGControl* ctrls_[GCTRL_SIZE];
		//std::vector<BaseGControl*> ctrls_;//全局控件集合 ,WIN=0, CTRLID
	};

}

