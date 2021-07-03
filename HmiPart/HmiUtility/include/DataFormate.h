#pragma once
#include "Sysdef.h"
#include "DataInfo.h"
#include "BaseVar.h"
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

namespace Utility
{
	class DataFormate 
	{
	public :
		static bool Logic(DDWORD& data, XJDataType& info);
		static std::string NumFmt(DDWORD& data, XJDataType& info, NumberFmtInfo& fmt);
		static DDWORD NumParse(std::string str, XJDataType& info);
		static DDWORD NumParse(DDWORD& data, XJDataType& info);
		static DDWORD BytesParse(int index, BYTE*  str, XJDataType& info);
		static int GetByteNumByInfo(XJDataType& info);
		static std::string StrFmt(std::string str, XJDataType & info, NumberFmtInfo & fmt);
		static bool BytesFmt(char* srcdata, char* desdata, int len, XJDataType& info);
		static DDWORD Add(DDWORD& data1, XJDataType& info1, DDWORD& data2, XJDataType& info2);
		static DDWORD Sub(DDWORD& data1, XJDataType& info1, DDWORD& data2, XJDataType& info2);
		static DDWORD Mul(DDWORD& data1, XJDataType& info1, DDWORD& data2, XJDataType& info2);
		static DDWORD Div(DDWORD& data1, XJDataType& info1, DDWORD& data2, XJDataType& info2);
		static int    Cmp(DDWORD& data1, XJDataType& info1, DDWORD& data2, XJDataType& info2);
		static void InitScaleInfo(ScaleInfo& scale, DDWORD& minData, XJDataType& minInfo, 
													DDWORD& maxData, XJDataType& maxInfo, 
													int min, int max);
		static int GetScale(ScaleInfo& scale, DDWORD& data, XJDataType& info);
		// 设置指定类型值
		template<typename T>
		static DDWORD SetNumber(T val, XJDataType& info) {
			DDWORD result;
			// 初始化数据类型
			memset(&result, 0x00, sizeof(result));
			switch (info.Cls)
			{
			case Project::DT_Bit:
			{
				// 转换成BOOL类型数据
				BOOL bVal = static_cast<BOOL>(val);
				// 赋值传出数值
				memcpy(&result, &bVal, sizeof(BOOL));
				break;
			}
			case Project::DT_Byte:
			{
				if (info.Type == Project::NT_BCD ||
					info.Type == Project::NT_Hex ||
					info.Type == Project::NT_Unsigned) {
					// 转换成无符号字符
					UINT8 ucVal = static_cast<UINT8>(val);
					// 赋值传出数值
					memcpy(&result, &ucVal, sizeof(UINT8));
				} else if (info.Type == Project::NT_Signed) {
					// 转换成带符号字符
					INT8 cVal = static_cast<INT8>(val);
					// 赋值传出数值
					memcpy(&result, &cVal, sizeof(INT8));
				}
				break;
			}
			case Project::DT_WORD:
			{
				if (info.Type == Project::NT_BCD ||
					info.Type == Project::NT_Hex ||
					info.Type == Project::NT_Unsigned) {
					// 转换成无符号短整型
					UINT16 usVal = static_cast<UINT16>(val);
					// 16位BCD类型范围为0-9999
					if (info.Type == Project::NT_BCD &&
						usVal > 9999) {
						usVal = 9999;
					}
					// 赋值传出数值
					memcpy(&result, &usVal, sizeof(UINT16));
				} else if (info.Type == Project::NT_Signed) {
					// 转换成带符号短整型
					INT16 sVal = static_cast<INT16>(val);
					// 赋值传出数值
					memcpy(&result, &sVal, sizeof(INT16));
				}
				break;
			}
			case Project::DT_DWORD:
			{
				if (info.Type == Project::NT_BCD ||
					info.Type == Project::NT_Hex ||
					info.Type == Project::NT_Unsigned) {
					// 转换成无符号整型
					UINT32 uiVal = static_cast<UINT32>(val);
					// 32位BCD类型范围为0-99999999
					if (info.Type == Project::NT_BCD &&
						uiVal > 99999999) {
						uiVal = 99999999;
					}
					// 赋值传出数值
					memcpy(&result, &uiVal, sizeof(UINT32));
				} else if (info.Type == Project::NT_Signed) {
					// 转换成有符号整型
					INT32 iVal = static_cast<INT32>(val);
					// 赋值传出数值
					memcpy(&result, &iVal, sizeof(INT32));
				} else if (info.Type == Project::NT_Float) {
					// 转换成单精度浮点数
					FLOAT fVal = static_cast<FLOAT>(val);
					// 赋值传出数值
					memcpy(&result, &fVal, sizeof(FLOAT));
				}
				break;
			}
			case Project::DT_DDWORD:
			{
				if (info.Type == Project::NT_BCD ||
					info.Type == Project::NT_Hex ||
					info.Type == Project::NT_Unsigned) {
					// 转换成无符号长整型
					UINT64 ulVal = static_cast<UINT64>(val);
					// 64位BCD类型范围为0-9999999999999999
					if (info.Type == Project::NT_BCD &&
						ulVal > 9999999999999999) {
						ulVal = 9999999999999999;
					}
					// 赋值传出数值
					memcpy(&result, &ulVal, sizeof(UINT64));
				} else if (info.Type == Project::NT_Signed) {
					// 转换成有符号长整型
					INT64 lVal = static_cast<INT64>(val);
					// 赋值传出数值
					memcpy(&result, &lVal, sizeof(INT64));
				} else if (info.Type == Project::NT_Float) {
					// 转换成双精度浮点数
					DOUBLE dVal = static_cast<DOUBLE>(val);
					// 赋值传出数值
					memcpy(&result, &dVal, sizeof(DOUBLE));
				}
				break;
			}
			default:
				break;
			}
			return result;
		}

		// 获取指定类型值
		template<typename T>
		static T GetNumber(const DDWORD& data,const XJDataType& info) {
			switch (info.Cls)
			{
			case Project::DT_Bit:
			{
				// 转换布尔值
				BOOL bVal = static_cast<BOOL>(data & 0xF);
				return static_cast<T>(bVal);
				break;
			}
			case Project::DT_Byte:
			{
				if (info.Type == Project::NT_BCD ||
					info.Type == Project::NT_Hex ||
					info.Type == Project::NT_Unsigned) {
					// 转换无符号字符
					UINT8 ucVal = static_cast<UINT8>(data & 0xFF);
					return static_cast<T>(ucVal);
				} else if (info.Type == Project::NT_Signed) {
					// 转换有符号字符
					INT8 cVal = static_cast<INT8>(data & 0xFF);
					return static_cast<T>(cVal);
				}
				break;
			}
			case Project::DT_WORD:
			{
				if (info.Type == Project::NT_BCD ||
					info.Type == Project::NT_Hex ||
					info.Type == Project::NT_Unsigned) {
					// 转换无符号短整型
					UINT16 usVal = static_cast<UINT16>(data & 0xFFFF);
					return static_cast<T>(usVal);
				} else if (info.Type == Project::NT_Signed) {
					// 转换有符号短整型
					INT16 sVal = static_cast<INT16>(data & 0xFFFF);
					return static_cast<T>(sVal);
				}
				break;
			}
			case Project::DT_DWORD:
			{
				if (info.Type == Project::NT_BCD ||
					info.Type == Project::NT_Hex ||
					info.Type == Project::NT_Unsigned) {
					// 转换无符号整型
					UINT32 uiVal = static_cast<UINT32>(data & 0xFFFFFFFF);
					return static_cast<T>(uiVal);
				} else if (info.Type == Project::NT_Signed) {
					// 转换有符号整型
					INT32 iVal = static_cast<INT32>(data & 0xFFFFFFFF);
					return static_cast<T>(iVal);
				} else if (info.Type == Project::NT_Float) {
					FLOAT fVal = { 0 };
					// 转换单精度浮点型
					memcpy(&fVal, &data, sizeof(FLOAT));
					return static_cast<T>(fVal);
				}
				break;
			}
			case Project::DT_DDWORD:
			{
				if (info.Type == Project::NT_BCD ||
					info.Type == Project::NT_Hex ||
					info.Type == Project::NT_Unsigned) {
					// 转换无符号长整型
					UINT64 ulVal = static_cast<UINT64>(data & 0xFFFFFFFFFFFFFFFF);
					return static_cast<T>(ulVal);
				} else if (info.Type == Project::NT_Signed) {
					// 有符号长整型
					INT64 lVal = static_cast<INT64>(data & 0xFFFFFFFFFFFFFFFF);
					return static_cast<T>(lVal);
				} else if (info.Type == Project::NT_Float) {
					DOUBLE dVal = { 0 };
					// 双精度浮点型
					memcpy(&dVal, &data, sizeof(DOUBLE));
					return static_cast<T>(dVal);
				}
				break;
			}
			default:
				break;
			}
			return 0;
		}
		 
	private:
		template<typename T>
		static int CalValue(DDWORD & data1, XJDataType& info1, DDWORD & data2, XJDataType& info2, T * result, char opt, bool isSigned);
		static DDWORD GetValue(DDWORD & data1, XJDataType& info1, DDWORD & data2, XJDataType& info2, char opt);
		template<typename T>
		static int Compare(T & a, T & b);
	};
}