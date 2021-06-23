#include "HirePurchaseControl.h"
#include "CHirePurchase.h"
#include "ResourceService.h"
#include "HMIWindow.h"
#include "UIData.h"
#include "SysCtrlApi.h"
#include <functional>

namespace UI
{
	HirePurchaseGControl *HirePurchaseGControl::ctrl_ = nullptr;

	HirePurchaseGControl::HirePurchaseGControl(HMIPage * page) : BaseGControl(page) {
		model_ = shared_ptr<HirePurchaseGModel>(new HirePurchaseGModel());
		InitMVCModel(model_);
		ctrl_ = this;
	}
	HirePurchaseGControl *HirePurchaseGControl::Ins()
	{
		return ctrl_;
	}

	HirePurchaseGControl::~HirePurchaseGControl() {

	}

	void HirePurchaseGControl::CreateView() {

		HirePurchaseGView* view = new HirePurchaseGView(0, 0, Page()->w(), Page()->h());

		InitMVCView(view);
	}

	void HirePurchaseGControl::OnReady()
	{
	}

	int HirePurchaseGControl::PeekHMIMessage(Message::Msg * msg)
	{
		return 1;
	}

	void HirePurchaseGControl::HandleDataNotify(Project::DataVarId id)
	{
		//rtip:应该绑定一个界面并接收密码信息
	}

}