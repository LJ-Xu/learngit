#pragma once
#include "Sysdef.h"
#include <string>
namespace Utility
{
			struct NumberTypeInfo
			{
				char Type;//bcd signed unsigned  float double 
			};
			struct StringTypeInfo
			{
				char Encode;
			};
			struct BytesTypeInfo
			{
				char Type;
			};
	struct DataTypeInfo
	{
		char Cls;//bit,byte,word,dword,ddword,//string,regs
		union {
            struct NumberTypeInfo Number;
            struct StringTypeInfo String;
            struct BytesTypeInfo Bytes;
		};
	};
	
 
	struct NumberFmtInfo
	{
		char Num1;	//整数个数
		char Num2;	//小数个数
		bool IsFillZero;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Num1, Num2, IsFillZero);
		}
	};
	struct NumberInfo
	{
		DataTypeInfo* TpInfo;
		NumberFmtInfo* FmtInfo;
	};
	struct ScaleInfo
	{
		double A;
		double B;
	};
}
