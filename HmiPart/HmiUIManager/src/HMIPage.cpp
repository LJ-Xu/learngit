#include "HMIWindow.h"
#include "HMIPage.h"
#include "System.h"
#include "BaseControl.h"
#include "IControlFactory.h"
#include "DataApi.h"
#include "ViewShowUtility.h"
#include "ControlFactory.h"
#include "KeyBtnControl.h"
namespace UI
{
	static void InitTimerParam(PageTimerParam *p)
	{
		p->Next = p;
		p->Prev = p;
	}
	HMIPage::HMIPage(int winid, int x, int y, int w, int h, const char* title) : Fl_Group(x, y, w, h, title)
	{
		winno_ = winid;
		//listQueue_.Init(100);
		InitTimerParam(&timerParamHead_);

	}
	void HMIPage::draw()
	{
		Fl_Color clr = color();
		//if (clr >> 24)
		fl_rectf(x(), y(), w(), h(), clr);
		
		Fl_Group::draw_children();
	}
#ifdef WIN32

	void HMIPage::remove(int index) {
		if (index < 0 || index >= children()) return;
		Fl_Widget &o = *child(index);

		if (&o == prefocus_) prefocus_ = 0;
		Fl_Group::remove(index);
	}

	void HMIPage::clear() {
		prefocus_ = 0;
		Fl_Group::clear();
	}
	int HMIPage::handle(int event)
	{
		extern Fl_Widget* fl_oldfocus; // set by Fl::focus

		switch (event) {

		case FL_FOCUS:
		{
			if (prefocus_ && prefocus_ != this)
				prefocus_->take_focus();
			return Fl_Group::handle(event);
		}
			
		case FL_UNFOCUS:
			prefocus_ = fl_oldfocus;
			return Fl_Group::handle(event);
		}
		return Fl_Group::handle(event);
	}
#endif // WIN32

	
	std::shared_ptr<BaseControl> HMIPage::NewCtr(Project::UnitInfo &unintInfo, UI::HMIPage* uipage)
	{
		std::shared_ptr<BaseControl> ctrl(IControlFactory::Ins()->GetNewControlByName(unintInfo.CtrName));
		if (ctrl)
		{
			ctrl->SetHMIPage(uipage);
			ctrl->CreateView(unintInfo.Data);
		}

		return ctrl;
	}

	void HMIPage::ProduceCtrl(Project::PageInfo& pi, int winno)
	{
		vector<Project::UnitInfo>& shapes = pi.Shapes;
		begin();
		if (pi.PageCfg.IsCloseBtn)		/*绘制关闭按钮*/
			ResizeCtrlBuf(shapes.size() + 1);
		else
			ResizeCtrlBuf(shapes.size());

		size_t i;
		for (i = 0; i < shapes.size(); i++)
		{
			std::shared_ptr<BaseControl> ctrl = NewCtr(shapes[i], this);
			ctrl->CtrlId(WINCTR_ID(winno, i));
			SetCtrl(i, ctrl);
		}
		if (pi.PageCfg.IsCloseBtn)		/*绘制关闭按钮*/
		{
			//Project::UnitInfo btnshape;
			//btnshape.CtrName = "KeyBtnControl";
			//std::shared_ptr<UI::BaseModel> mode = ctrl->GetMode();

			std::shared_ptr<BaseControl> ctrl(IControlFactory::Ins()->GetNewControlByName("KeyBtnControl"));
			shared_ptr<KeyBtnModel> model = std::static_pointer_cast<KeyBtnModel>(ctrl->GetMode());
			Project::StatusRes res;
			res.Texts.push_back("@+11+");
			model->KeyBtnConfig.Txt = res;
			vector<Project::StringStyle> styles;
			Project::StringStyle tmp;
			tmp.Colors = 0;
			tmp.Font.Name = "Arial";
			tmp.Font.Size = 14;
			tmp.Align = 5;
			styles.push_back(tmp);
			styles.push_back(tmp);
			model->KeyBtnConfig.StrStyles = styles;
			model->KeyBtnConfig.Width = 15;
			model->KeyBtnConfig.Height = 15;
			model->KeyBtnConfig.X = pi.Pos.X + pi.Sz.W - model->KeyBtnConfig.Width - 6;
			model->KeyBtnConfig.Y = pi.Pos.Y + 6;
			model->KeyBtnConfig.IsCloseBtn = true;
			ctrl->SetHMIPage(this);
			ctrl->CreateView("");
			ctrl->CtrlId(WINCTR_ID(winno, i));
			SetCtrl(i, ctrl);
		}
		end();
	}
	void HMIPage::ChangePos(int offx, int offy)
	{
		ChangePosEx(offx, offy);
		if(bottomPage_)
			bottomPage_->ChangePos(offx, offy);
		if (topPage_)
			topPage_->ChangePos(offx, offy);
	}
	HMIPage* HMIPage::FindPage(int winno)
	{
		HMIPage* pg = FindPageEx(winno);
		if (pg)
			return pg;
		if (bottomPage_)
		{
			pg = bottomPage_->FindPageEx(winno);
			if (pg)
				return pg;
		}
		if (topPage_)
		{
			pg = topPage_->FindPageEx(winno);
			if (pg)
				return pg;
		}
			
		return nullptr;
	}
	


	static void AddTimer(unsigned int ms, void* param)
	{
		Fl::add_timeout(ms / 1000.0, HMIPage::TimeOutHandler, param);
	}
	static void RemoveTimer(void* param)
	{
		Fl::remove_timeout(HMIPage::TimeOutHandler, param);
	}
	void HMIPage::AddTimeout(unsigned int ms, TimeoutHandler handler, void* param, bool isrepeat)
	{
		PageTimerParam* p = new PageTimerParam();
		p->Handler = handler;
		p->Param = param;
		if (isrepeat)
			p->Repeat = ms;
		p->Next = timerParamHead_.Next;
		p->Prev = &timerParamHead_;
		timerParamHead_.Next->Prev = p;
		timerParamHead_.Next = p;
		AddTimer(ms, p);
	}
	void HMIPage::RemoveTimeout(TimeoutHandler handler, void* param)
	{
		PageTimerParam* p = timerParamHead_.Next;
		while (p != &timerParamHead_)
		{
			if (p->Handler == handler && p->Param == param)
			{
				PageTimerParam* tmp = p->Next;
				p->Prev->Next = p->Next;
				if (p->Next)
					p->Next->Prev = p->Prev;
				Fl::remove_timeout(TimeOutHandler, p);
				delete p;
				p = tmp;
				break;
			}
			else
			{
				p = p->Next;
			}

		}
	}

	void HMIPage::TimeOutHandler(void* pm)
	{
		PageTimerParam* param = (PageTimerParam*)pm;
		param->Handler(param->Param);
		if (param->Repeat)
		{
			AddTimer(param->Repeat, param);
			return;
		}
		if (param->Prev)
		{
			param->Prev->Next = param->Next;
		}
		if (param->Next)
		{
			param->Next->Prev = param->Prev;
		}
		delete param;
	}

	bool HMIPage::NotifySysChange(SysChangeEM ctg)
	{
		if (!NotifySysChangeEx(ctg))
			return false;
		if (bottomPage_)
			bottomPage_->NotifySysChange(ctg);
		if (topPage_)
			topPage_->NotifySysChange(ctg);
		return true;
	}
	
	void HMIPage::NotifyCtrlClose()
	{
		for (size_t i = 0; i < ctrls_.size(); i++)
		{
			ctrls_[i]->OnClose();
		}
	}
	void HMIPage::Open()
	{
		OpenEx();
		if (bottomPage_)
			bottomPage_->Open();
		if (topPage_)
			topPage_->Open();
	}
	void HMIPage::Close()
	{
		CloseEx();
		if (bottomPage_)
			bottomPage_->Close();
		if (topPage_)
			topPage_->Close();
	}

	void HMIPage::Display(HMIWindow* win)
	{
		if (IsModal() && !modalpg_)
		{
			win->begin();
			modalpg_ = new ModalPage(0, 0, win->w(), win->h());

			//win->add(modalpg_);
			win->end();
			modalpg_->show();
			modalpg_->redraw();
		}
		win->add(this);
		show();
		redraw();
	}

	void HMIPage::Remove(HMIWindow* win)
	{
		if (IsModal() && modalpg_)
		{
			win->remove(modalpg_);
			delete modalpg_;
			modalpg_ = nullptr;
		}
		hide();
		win->remove(this);
	}

	void HMIPage::OpenEx()
	{
		DataApi::OnPageOpen(Winno());

		/*TODO:等待数据遍历一轮后才能通知Ready*/
		NotifyCtrlReadyEx();
		//NotifySysChange(SysChangeEM::Permission);
	}
	void HMIPage::CloseEx()
	{
		NotifyCtrlClose();
		CloseTimerEx();
		DataApi::OnPageClose(Winno());
	}
	bool HMIPage::NotifySysChangeEx(SysChangeEM ctg)
	{
		for (size_t i = 0; i < ctrls_.size(); i++)
		{
			if(!ctrls_[i]->HandleSysChange(ctg))
				return false;
		}
		return true;
	}
	void HMIPage::ChangePosEx(int offx, int offy)
	{
		pageoffx_ = offx;
		pageoffy_ = offy;
		x(offx); y(offy);
		for (size_t i = 0; i < ctrls_.size(); i++)
		{
			ctrls_[i]->SetOffSetPos(pageoffx_, pageoffy_);
			HMIBaseView* view = ctrls_[i]->GetView();
			if (view)
				view->position(view->x() + offx, offy + view->y());
			//UI::ViewShowUtility::ShowView(view, true, offx + view->x(), offy + view->y());
		}
		//redraw();
	}
	void HMIPage::NotifyCtrlReadyEx()
	{
		for (size_t i = 0; i < ctrls_.size(); i++)
		{
			ctrls_[i]->OnReady();
		}
	}


	HMIPage* HMIPage::FindPageEx(int winno)
	{
		if (winno == winno_)
			return this;
		if (topPage_&&topPage_->Winno() == winno)
			return topPage_;
		if (bottomPage_&&bottomPage_->Winno() == winno)
			return bottomPage_;
		return nullptr;
	}
	void HMIPage::CloseTimerEx()
	{
		PageTimerParam* p = timerParamHead_.Next;
		while (p != &timerParamHead_)
		{
			PageTimerParam* tmp = p->Next;
			Fl::remove_timeout(TimeOutHandler, p);
			delete p;
			p = tmp;
		}
	}
	/*int HMIPage::PeekHMIMessage(Message::Msg* msg)
	{

	}*/

	
}


