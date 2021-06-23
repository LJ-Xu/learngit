/*******************************************************************************
 * Copyright (C) 2011-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * FileName	: BaseControl.h
 * Author	: zhanglinbo
 * Descript	: ��������Control
 * Version	: 0.1
 * Modify	:
 *			:
 *******************************************************************************/
#pragma once
#ifdef WIN32
#include "stdafx.h"
#include "UIComm.h"
#include "CtrlID.h"
#include "BaseModel.h"
#include "HMIPage.h"
#else
#include "stdafx.h"
#include "HMIPage.h"
#include "UIComm.h"
#include "CtrlID.h"
#include "BaseModel.h"
#endif // WIN32


namespace Message
{
	struct Msg;
}
namespace Project
{
	struct BaseVar;
	struct DataVarId;
}
class HMIBaseView;
namespace UI
{
	class BaseModel;
	class BaseControl :public std::enable_shared_from_this<BaseControl>
	{
	public:
		BaseControl() {};
		BaseControl(HMIPage* pg) { pPage_ = pg; };
		virtual ~BaseControl() {};

		/******************************************************/
		void SetHMIPage(HMIPage* pg) { pPage_ = pg; };
		virtual void GetRect(Rectangle& rect)
		{
			pModel_->GetRect(rect);
		}
		virtual void SetRect(int x, int y, int w, int h)
		{
			pModel_->SetRect(x, y, w, h);
			if (pView_->x() != x || pView_->y() != y)
			{
				pView_->position(x, y);
			}
			if (pView_->w() != w || pView_->h() != h)
			{
				pView_->size(w, h);
			}
		}
		bool IsInvalidate();
		void Invalidate(Rectangle& rect);

		virtual HMIBaseView* CreateView(std::string json)
		{
			pModel_->InitData(json);
			CreateView();
			return pView_;
		}
		virtual HMIBaseView* CreateView(std::vector<char>& data)
		{
			pModel_->InitData(data);
			CreateView();
			return pView_;
		}
		virtual void Render();
		virtual int PeekHMIMessage(Message::Msg* msg) = 0;
		virtual void SetOffSetPos(int offx, int offy) {}

		virtual void HandleSysChange(SysChangeEM catogray)
		{

		}
		Project::WinCtrlID CtrlId() { return ctrlId_; };   //�ؼ�ID
		void CtrlId(Project::WinCtrlID id) { ctrlId_ = id; };   //�ؼ�ID
		std::shared_ptr<BaseModel> GetMode()
		{
			return pModel_;
		}
		HMIBaseView* GetView()
		{
			return pView_;
		}
		HMIWindow*  Win()
		{
			return pPage_->Win();
		}
		HMIPage* Page()
		{
			return pPage_;
		}
		virtual void OnReady() {} //��ʼ
		virtual void OnClose() {}//�ر�

		void TestInitView()
		{
			CreateView();
		}
	protected:
		virtual void CreateView() = 0;
		HMIPage* pPage_ = nullptr;
		std::shared_ptr<BaseModel> pModel_;
		//std::shared_ptr<HMIBaseView> pView_;
		HMIBaseView* pView_ = nullptr;

		template <class M> void InitMVCModel(std::shared_ptr<M> pMode)
		{
			//��ʼ��MODEL
			pModel_ = pMode;
		}
		template <class T> void InitMVCView(T* pView)
		{
			//������ctrl��Ŀǰ����Fl_Widget�����
			pView->BaseView.SetCtrlModel(shared_from_this(), pModel_);
			//��ʼ��VIEW
			pView_ = (HMIBaseView*)pView;
		}

	private:
		Project::WinCtrlID ctrlId_ = -1;
	};
}
