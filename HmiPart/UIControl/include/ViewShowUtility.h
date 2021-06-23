/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ViewShowUtility.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : øÿº˛÷ÿªÊ¿‡
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "UIComm.h"
#include "Permission.h"
namespace UI
{
	class ViewShowUtility
	{
	public:
		static void ShowView(HMIBaseView *view, Project::Permission& perm,int x = -1,int y = -1);
	};
}



