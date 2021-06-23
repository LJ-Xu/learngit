/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : NoticesUtility.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 写入前、写入后通知处理
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "NoticesUtility.h"
#include "UIData.h"

namespace UI
{
	void NoticesUtility::NoticeWrite(Project::NoticesAction& notice)
	{
		if (notice.Bit.DataBitVar != Project::DataVarId::NullId)
		{
			//通知位
			switch (notice.Bit.State)
			{
			case Project::SetON:
				UI::UIData::Bit(notice.Bit.DataBitVar, true);
				break;
			case Project::SetOFF:
				UI::UIData::Bit(notice.Bit.DataBitVar, false);
				break;
			case Project::Rising:
				UI::UIData::Bit(notice.Bit.DataBitVar, false);
				UI::UIData::Bit(notice.Bit.DataBitVar, true);
				break;
			case Project::Falling:
				UI::UIData::Bit(notice.Bit.DataBitVar, true);
				UI::UIData::Bit(notice.Bit.DataBitVar, false);
				break;
			default:
				break;
			}
		}
		if (notice.Word.DataWordVar != Project::DataVarId::NullId)		//通知字
			UI::UIData::Number(notice.Word.DataWordVar, notice.Word.Val);
	}
}
