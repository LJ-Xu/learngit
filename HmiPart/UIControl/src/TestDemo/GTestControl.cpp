#include "stdafx.h"
#include "Message.h"
#include "HMIWindow.h"
#include "GTestControl.h"
#include "GTestView.h"
#include "UIData.h"

namespace UI
{
	GTestControl::GTestControl(HMIPage* w) :BaseControl(w)
	{
		//创建GTestModel
		std::shared_ptr <GTestModel> mode(new GTestModel());
		InitMVCModel(mode);
	}
	void ClickCB(Fl_Widget* view ,void* param)
	{
		HMIPage* pg = (HMIPage*)param;
		GTestView *v = (GTestView *)view;
		std::shared_ptr <GTestModel> md = v->BaseView.GetModel<GTestModel>();
		static int status = 0;
		static short val = 10;
		switch (md->Config.FuncID)
		{
		case Project::FuncType::CloseDialog:
			pg->Win()->ClosePage(2);
			break;
		case Project::FuncType::OpenDialog:
			pg->Win()->OpenDialogPage(2);
			break;
		case Project::FuncType::StringF:		
			//val = (val + 10) % 100;
			val = (val + 1) % 10;
			UIData::Number(md->Config.Val, val);
			v->label(IResourceService::Ins()->GetString(md->Config.Title, status));
			status = !status;
			v->redraw();

			break;

		case Project::FuncType::AddValueF:
			++val;
			UIDataService::Ins().SetNumber(md->Config.Val, val);
			v->label(IResourceService::Ins()->GetString(md->Config.Title, status));
			status = !status;
			v->redraw();
			break;
		case Project::FuncType::SubValueF:
			--val;
			UIDataService::Ins().SetNumber(md->Config.Val, val);
			v->label(IResourceService::Ins()->GetString(md->Config.Title, status));
			status = !status;
			v->redraw();
			break;
		case Project::FuncType::TimerF:	
		{
			pg->AddTimeout(1000, [](void* v) {
				thread::id threadId = this_thread::get_id();
				cout << threadId;
				GTestView* view = (GTestView *)v;
				view->color(FL_RED);
				view->redraw();
			}, view);
		}	
			break;
		default:
			break;
		}


		
	 
	}

	void GTestControl::CreateView()
	{
		//初始化VIEW
		std::shared_ptr <GTestModel> mode = std::static_pointer_cast<GTestModel>(pModel_);
		GTestView* view = new GTestView(mode->Config.X, mode->Config.Y,
			mode->Config.Width, mode->Config.Height, 
			IResourceService::Ins()->GetString(mode->Config.Title, 0));
		InitMVCView(view);
		view->callback(ClickCB, Page());
		// view->when(FL_WHEN_RELEASE);
	}

	int GTestControl::PeekHMIMessage(Message::Msg* msg)
	{
		std::shared_ptr <GTestModel> mode = std::static_pointer_cast<GTestModel>(pModel_);
		GTestView* view = (GTestView*)pView_;
		switch (msg->Code)
		{
		case WM_EVENT_DATEUPDATE:
			if (msg->LParam == mode->Config.VOffX.Vid)
			{
				int x = mode->Config.X + UIDataService::Ins().GetNumber<WORD>(mode->Config.VOffX);
				view->hide();
				view->position(x,view->y());
				view->show();
				//view->redraw();
			}
			break;
		default:
			break;
		}
		return 1;//handled;
	}

}
