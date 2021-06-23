#pragma once
#include <string>
#include "Sysdef.h"
namespace UI
{
	class CodeFormatUtility
	{
	public:
		enum CodeRule :char
		{
			ASCII,
			UTF_8,
			UTF_16,
			GB2312
		};
		static bool IsStrUtf8(const char* str);
		static bool IsStrGbk(const char* str);

		/*±àÂë×ª³ÉUtf8*/
		static bool CodeToUtf8(CodeRule rule, char *data, int len);
		static bool Utf8ToCode(CodeRule rule, char *data, int len);
		static std::string CodeConversion(std::string src, CodeRule rule);
		static std::string EndianSwap(std::string);

		static int GB2312ToUtf8(char*  ptDestText,
			int  nDestLength,
			char*  ptSrcText,
			int  nSrcLength);

		static int Utf8ToGB2312(char *gb2312_str,
			int len_gb2312,
			char *utf8_str,
			int len_utf8);
		
	};
}

