#include "stdafx.h"
#include "BaseControl.h"
#include "BasicView.h"
#include "HMIWindow.h"
namespace UI
{
	void BaseControl::Render()
	{
		if (pView_ != nullptr)pView_->draw();
	}
 
	bool BaseControl::IsInvalidate()
	{
		return pView_->damage()== FL_DAMAGE_USER1;
	}
	void BaseControl::Invalidate(Rectangle& rect)
	{
		//pView_->redraw();
		
		pView_->damage(FL_DAMAGE_USER1, rect.X, rect.Y, rect.W, rect.H);
		//pView_->damage(FL_DAMAGE_CHILD/*FL_DAMAGE_ALL*/, rect.X, rect.Y,rect.W,rect.H);//FL_DAMAGE_ALL
	}
 
 
 
}
