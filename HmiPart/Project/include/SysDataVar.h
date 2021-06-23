#pragma once
#include "stdafx.h"
#include "BaseVar.h"

class SysDataVar : public BaseVar
{
public:
	INT32  count; //数量
	INT32  rawAddr; //间接指定需要知道原始的地址
	BaseVar* offset;//是否间接指定	
	//INT32   no;   //编号
	//INT32   count;//读取的数据个数
	template <typename T>
	T GetNumberValue(); //获取数值
	std::string GetString();//获取字符串
};
template<typename T>
inline T SysDataVar::GetNumberValue()
{
	//T* ptr = reinterpret_cast<T>(&outData.load().numData);
	//return *ptr;
	return default(T);
}

