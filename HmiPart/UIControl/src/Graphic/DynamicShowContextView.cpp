#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "UIComm.h"
#include "DynamicShowContextView.h"
#include "DynamicShowContextControl.h"
#include "DynamicShowContextModel.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include "System.h"
#include <math.h>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Scroll.H>
namespace UI
{
	DynamicShowContextView::DynamicShowContextView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H), Inited(false), CurDx(0), MainStrLen(0), CurAlarmIndex(0)
	{
		memset(TitleTip, 0, 5);
	}

	DynamicShowContextView::~DynamicShowContextView()
	{
	}

	void DynamicTextContextView::draw()
	{
		shared_ptr<DynamicShowContextControl> ctrl = BaseView.GetControl<DynamicShowContextControl>();
		shared_ptr<DynamicShowContextModel> model = BaseView.GetModel<DynamicShowContextModel>();
		FinX = model->DynamicTextBarUnit.X + model->DynamicTextBarUnit.OffX;
		FinY = model->DynamicTextBarUnit.Y + model->DynamicTextBarUnit.OffY;
		for (int index = 0; i < ContextStrList.size(); ++i)
		{
			//ContextStrList[i]
		}
	}
}