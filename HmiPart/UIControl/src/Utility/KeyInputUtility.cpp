#include "KeyInputUtility.h"
namespace UI
{
	bool KeyInputUtility::ischinesestate_ = false;
	bool KeyInputUtility::isupperstate_  = false;

	bool  KeyInputUtility::IsUpper(void)
	{
		return isupperstate_;
	}
	bool  KeyInputUtility::IsChinese(void)
	{
		return ischinesestate_;
	}
	void  KeyInputUtility::SetUpperState(void)
	{
		isupperstate_ = !isupperstate_;
	}
	void KeyInputUtility::SetUpperState(bool status)
	{
		isupperstate_ = status;
	}

	void  KeyInputUtility::SetChineseState(void)
	{
		ischinesestate_ = !ischinesestate_;
	}
}