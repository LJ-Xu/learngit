/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ViewShowUtility
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ²¿¼şÖØ»æ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "ViewShowUtility.h"
#include "UIData.h"

namespace UI
{
	void ViewShowUtility::ShowView(HMIBaseView *view, Project::Permission& perm, int x, int y)
	{
		int prex = view->x();
		int prey = view->y();
		if (prex != x || prey != y)
		{
			view->position(x, y);
			if (perm.HasShowPerm && perm.HasLimitShowPerm)
			{
				view->hide();
				view->redraw();
				view->show();
			}
		}
		else
		{
			if (perm.HasShowPerm && perm.HasLimitShowPerm)
				view->redraw();
		}
	}
}