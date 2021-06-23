#include "UIData.h"
namespace UI
{
	bool UIData::Bit(Project::DataVarId& vid)
	{
		//return UIDataService::Ins().GetBit(vid);
		//return UIDataService::Ins().GetNumber(vid);
		return DataApi::GetBit(vid);

	}

	void UIData::Bit(Project::DataVarId& vid, bool val)
	{
		//UIDataService::Ins().SetBit(vid, val);
		/*int data = val;
		UIDataService::Ins().SetNumber(vid, data);*/

		return DataApi::SetBit(vid,val);
	}
}



