#pragma once
#include "stdafx.h"
#include "BaseVar.h"

class SysDataVar : public BaseVar
{
public:
	INT32  count; //����
	INT32  rawAddr; //���ָ����Ҫ֪��ԭʼ�ĵ�ַ
	BaseVar* offset;//�Ƿ���ָ��	
	//INT32   no;   //���
	//INT32   count;//��ȡ�����ݸ���
	template <typename T>
	T GetNumberValue(); //��ȡ��ֵ
	std::string GetString();//��ȡ�ַ���
};
template<typename T>
inline T SysDataVar::GetNumberValue()
{
	//T* ptr = reinterpret_cast<T>(&outData.load().numData);
	//return *ptr;
	return default(T);
}

