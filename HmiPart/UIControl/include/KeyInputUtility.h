#pragma once
namespace UI
{
	class KeyInputUtility
	{
	public:
		static bool IsUpper(void);
		static bool IsChinese(void);
		static void SetUpperState(void);
		static void SetUpperState(bool status);
		static void SetChineseState(void);
	private:
		static bool isupperstate_;
		static bool ischinesestate_;
	};
}

