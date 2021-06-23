#pragma once
#include "BaseGControl.h"
#include "HirePurchaseModel.h"
#include "HirePurchaseView.h"
#include <BaseControl.h>
#include <vector>
using namespace std;

namespace UI
{
	class HirePurchaseGControl : public BaseGControl
	{
	public:
		HirePurchaseGControl() : BaseGControl(nullptr) { }
		HirePurchaseGControl(HMIPage *);
		~HirePurchaseGControl();
		bool Changeflag = false;
	public:
		void CreateView() override;
		void OnReady() override;
		int PeekHMIMessage(Message::Msg * msg);
		void HandleDataNotify(Project::DataVarId id);
		static  HirePurchaseGControl  *Ins();
	private:
		shared_ptr<HirePurchaseGModel> model_;
		BOOL currentState = false;
		int index = 0;
		static HirePurchaseGControl *ctrl_;

	};
}
