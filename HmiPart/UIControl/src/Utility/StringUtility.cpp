/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StringUtility.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ×Ö·û´®»ñÈ¡
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "IResourceService.h"
#include "StringUtility.h"
namespace UI
{
 
	std::string StringUtility::GetDrawString(IResourceService* srv, Project::StatusRes& res, int status)
	{
		string label = "";
		int i, size;
		size = res.Texts.size();
		if (size == 0)
			return label;
		if (!res.Label.empty())
			label = srv->GetString(res.Label, status);
		//label = ResourceService::Ins().GetString("Label_1",0,0);
		else if (res.TableNo >= 0 && res.RowNo >= 0)
			label = srv->GetString(res.TableNo, 0, res.RowNo);
		else
		{
			std::unique_ptr<std::string[]> texts{ new std::string[size] };
			vector<std::string>::iterator currentText;
			for (i = 0, currentText = res.Texts.begin(); currentText != res.Texts.end(); currentText++, i++)
				texts.get()[i] = (*currentText);
			if ((size_t)status >= res.Texts.size())
				return label;
			label = texts.get()[status];
		}
		return label;
	}
}
