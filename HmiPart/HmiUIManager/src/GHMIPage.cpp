#include "GHMIPage.h"
#include "IControlFactory.h"
namespace UI
{
	GHMIPage::GHMIPage(int pageid,int x, int y, int w, int h, const char* title):HMIPage(0,x,y,w,h,title)
	{
		/*for (size_t i = 0; i < GCtrlEnum::GCTRL_SIZE; i++)
		{
			ctrls_[i] = IControlFactory::Ins()->GetNewGControlByID(i).release();
		}*/
	}
	GHMIPage::~GHMIPage()
	{
	}
 
	
	///目前在DataService线程里处理
	///数据采集和报警
	void GHMIPage::HandleDataNotify(int ctrlidx, Project::DataVarId vid)
	{
		/*ctrls_[ctrlidx]->HandleDataNotify(vid);*/		
		std::shared_ptr<BaseGControl> ptr = static_pointer_cast<BaseGControl>(ctrls_[ctrlidx]);
		if(ptr)
			ptr->HandleDataNotify(vid);
	}
}

