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
		//����UI�߳�ִ�еĺ���
		void HandleDataNotify(int ctrlidx, Project::DataVarId id);
	private:
		//BaseGControl* ctrls_[GCTRL_SIZE];
		//std::vector<BaseGControl*> ctrls_;//ȫ�ֿؼ����� ,WIN=0, CTRLID
	};

}

