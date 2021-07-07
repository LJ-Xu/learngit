#include "DataFormate.h"

namespace Utility
{
	bool DataFormate::Logic(DDWORD& data, XJDataType& info)
	{
		return true;
	}

	std::string DataFormate::NumFmt(DDWORD& data, XJDataType& info, NumberFmtInfo& fmt)
	{
		stringstream ss;
		//double a = Utility::DataFormate::GetNumber<double>(data, info);

		////double a = data;
		//DDWORD da;
		//memcpy(&da, &a, sizeof(DDWORD));
		double db = *(double*)&data;
		
		switch (info.Cls)
		{
		case Project::DT_Byte:
			if (info.Type == Project::NT_Signed) {
				INT8 v = (INT8)db;
				ss << v;
			}
			else if (info.Type == Project::NT_Hex) {
				UINT16 v = (UINT8)db;
				ss << std::hex << v;
				//ss << hex << v;
			}
			else if (info.Type == Project::NT_BCD) {
				UINT16 v = (UINT8)db;
				ss << std::hex << v;

				//ss << hex << v;
				string str = ss.str();
				for (size_t i = 0; i < str.size(); i++)
				{
					if ((str[i] >= 'A'&& str[i] <= 'F') || (str[i] >= 'a'&& str[i] <= 'f'))
					{
						string data = string(fmt.Num1, '*');
						return data;
					}
				}
			}
			else {
				UINT8 v = (UINT8)db;
				ss << v;
			}
			break;
		case Project::DT_WORD:
			if (info.Type == Project::NT_Signed) {
				INT16 v = (INT16)db;
				ss << v;
				//ss << GetNumber<INT16>(data, info);
			} else if (info.Type == Project::NT_Hex) {
				UINT16 v = (UINT16)db;
				ss << hex << v;
			}
			else if (info.Type == Project::NT_BCD) {
				UINT16 v = (UINT16)db;
				ss << hex << v;
				string str = ss.str();
				for (size_t i = 0; i < str.size(); i++)
				{
					if ((str[i] >= 'A'&& str[i] <= 'F') || (str[i] >= 'a'&& str[i] <= 'f'))
					{
						string data = string(fmt.Num1, '*');
						return data;
					}
				}
			}else {
				UINT16 v = (UINT16)db;
				ss << v;
				//ss << GetNumber<UINT16>(data, info);
			}
			break;
		case Project::DT_DWORD:
			if (info.Type == Project::NT_Float) {	// Float
				FLOAT val = (FLOAT)db;
				FLOAT sub = val - (INT32)val;
				ss << (INT32)val;
				if (sub > 0) {
					ss << setprecision(fmt.Num2) << sub;
				}
			} else if (info.Type == Project::NT_Signed) {
				INT32 val = (INT32)db;
				ss << (INT32)val;
			} else if (info.Type == Project::NT_Hex) {
				INT32 val = (INT32)db;
				ss << hex << (INT32)val;
			}
			else if (info.Type == Project::NT_BCD) {
				INT32 v = (INT32)db;
				ss << hex << v;
				string str = ss.str();
				for (size_t i = 0; i < str.size(); i++)
				{
					if ((str[i] >= 'A'&& str[i] <= 'F') || (str[i] >= 'a'&& str[i] <= 'f'))
					{
						string data = string(fmt.Num1, '*');
						return data;
					}
				}
			}
			else {
				INT32 val = (INT32)db;
				ss << (INT32)val;
			}
			break;
		case Project::DT_DDWORD:
			if (info.Type == Project::NT_Float) {	// Double
				DOUBLE val = (DOUBLE)db;
				DOUBLE sub = val - (INT64)val;
				ss << (INT64)val;
				if (sub > 0) {
					ss << setprecision(fmt.Num2) << sub;
				}
			} else if (info.Type == Project::NT_Signed) {

				ss << INT64(db);
			} else if (info.Type == Project::NT_Hex) {
				ss << hex<< UINT64(db);
			}
			else if (info.Type == Project::NT_BCD) {
				UINT64 v = (UINT64)db;
				ss << hex << v;
				string str = ss.str();
				for (size_t i = 0; i < str.size(); i++)
				{
					if ((str[i] >= 'A'&& str[i] <= 'F') || (str[i] >= 'a'&& str[i] <= 'f'))
					{
						string data = string(fmt.Num1, '*');
						return data;
					}
				}
			}
			else {
				ss << UINT64(db);
			}
			break;
		default:
			break;
		}
		string str = ss.str();
		int pos = str.find('.');
		if (pos != string::npos) {
			str = str.erase(pos - 1, 1);
			pos--;
		} else {
			pos = str.length();
			if (fmt.Num2 > 0) {
				str = str + '.' + string(fmt.Num2, '0');
			}
		}
		// 限制整数位个数
		if (pos > fmt.Num1) {
			str = str.erase(0, pos - fmt.Num1);
		}
		// 是否前导0
		if (fmt.IsFillZero && pos < fmt.Num1) {
			str = string(fmt.Num1 - pos, '0') + str;
		}
		return str;
	}
	DDWORD DataFormate::NumParse(DDWORD& data, XJDataType& info)
	{
		double db = *(double*)&data;
		DDWORD result;
		// 初始化数值
		memset(&result, 0x00, sizeof(DDWORD));
		switch (info.Cls)
		{
		case Project::DT_Byte:
			if (info.Type == Project::NT_Signed) {
				INT8 v = (INT8)db;
				memcpy(&result, &v, sizeof(INT8));
			}
			else {
				UINT8 v = (UINT8)db;
				memcpy(&result, &v, sizeof(INT8));
			}
			break;
		case Project::DT_WORD:
			if (info.Type == Project::NT_Signed) {
				INT16 v = (INT16)db;
				memcpy(&result, &v, sizeof(INT16));
			}
			else if (info.Type == Project::NT_Hex) {
				UINT16 v = (UINT16)db;
				memcpy(&result, &v, sizeof(INT16));
			}
			else {
				UINT16 v = (UINT16)db;
				memcpy(&result, &v, sizeof(INT16));
				//ss << GetNumber<UINT16>(data, info);
			}
			break;
		case Project::DT_DWORD:
			if (info.Type == Project::NT_Float) {	// Float
				FLOAT val = (FLOAT)db;
				memcpy(&result, &val, sizeof(FLOAT));
			}
			else if (info.Type == Project::NT_Signed) {
				INT32 val = (INT32)db;
				memcpy(&result, &val, sizeof(INT32));
			}
			else if (info.Type == Project::NT_Hex) {
				INT32 val = (INT32)db;
				memcpy(&result, &val, sizeof(INT32));
			}
			else {
				INT32 val = (INT32)db;
				memcpy(&result, &val, sizeof(INT32));
			}
			break;
		case Project::DT_DDWORD:
			if (info.Type == Project::NT_Float) {	// Double
				DOUBLE val = db;
				memcpy(&result, &val, sizeof(DOUBLE));
			}
			else if (info.Type == Project::NT_Signed) {
				memcpy(&result, &db, sizeof(INT64));
			}
			else if (info.Type == Project::NT_Hex) {
				memcpy(&result, &db, sizeof(INT64));
			}
			else {
				memcpy(&result, &db, sizeof(INT64));
			}
			break;
		default:
			break;
		}
		return result;
	}

	DDWORD DataFormate::NumParse(std::string str, XJDataType& info)
	{
		DDWORD result;
		// 初始化数值
		memset(&result, 0x00, sizeof(DDWORD));
		stringstream ss;
		ss.str(str);
		switch (info.Cls)
		{
		case Project::DT_Byte:
			if (info.Type == Project::NT_Unsigned) {
				UINT8 ret = { 0 };
				ss >> ret;
				memcpy(&result, &ret, sizeof(UINT8));
			} else if (info.Type == Project::NT_Signed) {
				INT8 ret = { 0 };
				ss >> ret;
				memcpy(&result, &ret, sizeof(INT8));
			}
			break;
		case Project::DT_WORD:
			if (info.Type == Project::NT_BCD) {
				UINT16 ret = { 0 };
				ss >> ret;
				if (ret > 9999)
					ret = 9999;
				memcpy(&result, &ret, sizeof(UINT16));
			} else if (info.Type == Project::NT_Hex) {
				UINT16 ret = { 0 };
				ss >> hex >> ret;
				memcpy(&result, &ret, sizeof(UINT16));
			} else if (info.Type == Project::NT_Unsigned) {
				UINT16 ret = { 0 };
				ss >> ret;
				memcpy(&result, &ret, sizeof(UINT16));
			} else if (info.Type == Project::NT_Signed) {
				INT16 ret = { 0 };
				ss >> ret;
				memcpy(&result, &ret, sizeof(INT16));
			}
			break;
		case Project::DT_DWORD:
			if (info.Type == Project::NT_BCD) {
				UINT32 ret = { 0 };
				ss >> ret;
				if (ret > 99999999)
					ret = 99999999;
				memcpy(&result, &ret, sizeof(UINT32));
			} else if (info.Type == Project::NT_Hex) {
				UINT32 ret = { 0 };
				ss >> hex >> ret;
				memcpy(&result, &ret, sizeof(UINT32));
			} else if (info.Type == Project::NT_Unsigned) {
				UINT32 ret = { 0 };
				ss >> ret;
				memcpy(&result, &ret, sizeof(UINT32));
			} else if (info.Type == Project::NT_Signed) {
				INT32 ret = { 0 };
				ss >> ret;
				memcpy(&result, &ret, sizeof(INT32));
			} else if (info.Type == Project::NT_Float) {
				FLOAT ret = { 0 };
				ret = (FLOAT)atof(str.c_str());
				memcpy(&result, &ret, sizeof(FLOAT));
			}
			break;
		case Project::DT_DDWORD:
			if (info.Type == Project::NT_BCD) {
				UINT64 ret = { 0 };
				ss >> ret;
				if (ret > 9999999999999999)
					ret = 9999999999999999;
				memcpy(&result, &ret, sizeof(UINT64));
			} else if (info.Type == Project::NT_Hex) {
				UINT64 ret = { 0 };
				ss >> hex >> ret;
				memcpy(&result, &ret, sizeof(UINT64));
			} else if (info.Type == Project::NT_Unsigned) {
				UINT64 ret = { 0 };
				ss >> ret;
				memcpy(&result, &ret, sizeof(UINT64));
			} else if (info.Type == Project::NT_Signed) {
				INT64 ret = { 0 };
				ss >> ret;
				memcpy(&result, &ret, sizeof(INT64));
			} else if (info.Type == Project::NT_Float) {
				DOUBLE ret = { 0 };
				ret = atof(str.c_str());
				memcpy(&result, &ret, sizeof(DOUBLE));
			}
			break;
		default:
			break;
		}
		return result;
	}
	DDWORD DataFormate::BytesParse(int index, BYTE* str, XJDataType& info)
	{
		DDWORD result;
		// 初始化数值
		memset(&result, 0x00, sizeof(DDWORD));
		stringstream ss;
		switch (info.Cls)
		{
		case Project::DT_Byte:
			memcpy(&result, &str[index * sizeof(UINT8)], sizeof(UINT8));
			break;
		case Project::DT_WORD:
			memcpy(&result, &str[index * sizeof(UINT16)], sizeof(UINT16));
			break;
		case Project::DT_DWORD:
			memcpy(&result, &str[index * sizeof(UINT32)], sizeof(UINT32));
			break;
		case Project::DT_DDWORD:
			memcpy(&result, &str[index * sizeof(UINT64)], sizeof(UINT64));
			break;
		default:
			break;
		}
		return result;
	}
	int DataFormate::GetByteNumByInfo(XJDataType& info)
	{
		int num = 0;
		switch (info.Cls)
		{
		case Project::DT_Byte:
			num =  sizeof(UINT8);
			break;
		case Project::DT_WORD:
			num = sizeof(UINT16);
			break;
		case Project::DT_DWORD:
			num = sizeof(UINT32);
			break;
		case Project::DT_DDWORD:
			num = sizeof(UINT64);
			break;
		default:
			break;
		}
		return num;
	}

	std::string DataFormate::StrFmt(std::string str, XJDataType & info, NumberFmtInfo & fmt)
	{
		DDWORD val = NumParse(str, info);
		return NumFmt(val, info, fmt);
	}

	bool DataFormate::BytesFmt(char* srcdata, char* desdata, int len, XJDataType& info)
	{
		return false;
	}

	// 计算值
	template<typename T>
	int DataFormate::CalValue(DDWORD & data1, XJDataType& info1, DDWORD & data2, XJDataType& info2, T * result, char opt, bool isSigned) {
		if (result == nullptr)
			return -1;
		// 清空结果值
		memset(result, 0x00, sizeof(T));
		// 获取计算值
		T val1 = GetNumber<T>(data1, info1);
		T val2 = GetNumber<T>(data2, info2);
		// 获取计算结果
		switch (opt)
		{
		case '+':
			*result = static_cast<T>(val1 + val2);
			// 判断值是否溢出
			if (!strcmp(typeid(T).name(), "float") || 
				!strcmp(typeid(T).name(), "double")) {
				if (*result > numeric_limits<T>::max()) {
					*result = numeric_limits<T>::max();
				} else if (*result < numeric_limits<T>::min()) {
					*result = numeric_limits<T>::min();
				}
			} else if (isSigned) {
				if (val1 >= 0 && val2 >= 0 && *result < 0) {
					*result = numeric_limits<T>::max();
				} else if (val1 < 0 && val2 < 0 && *result >= 0) {
					*result = numeric_limits<T>::min();
				}
			} else {
				if (val1 > *result && val2 > *result) {
					*result = numeric_limits<T>::max();
				}
			}
			break;
		case '-':
			*result = static_cast<T>(val1 - val2);
			// 判断值是否溢出
			if (isSigned) {
				if (val2 == numeric_limits<T>::min() && val1 >= 0) {
					*result = numeric_limits<T>::min();
				}
			} else {
				if (val1 < val2) {
					*result = numeric_limits<T>::min();
				}
			}
			break;
		case '*':
			if (val1 == 0 || val2 == 0) {
				*result = 0;
				break;
			}
			*result = static_cast<T>(val1 * val2);
			// 判断值是否溢出
			if (*result / val2 != val1) {
				if ((val1 < 0 && val2 < 0) || (val1 > 0 && val2 > 0)) {
					*result = numeric_limits<T>::max();
				} else {
					*result = numeric_limits<T>::min();
				}
			}
			break;
		case '/':
			if (val1 == 0 || val2 == 0) {
				*result = 0;
				break;
			}
			*result = static_cast<T>(val1 / val2);
			// 判断值是否溢出
			if (*result * val2 != val1) {
				if ((val1 < 0 && val2 < 0) || (val1 > 0 && val2 > 0)) {
					*result = numeric_limits<T>::max();
				}
				else {
					*result = numeric_limits<T>::min();
				}
			}
			break;
		default:
			break;
		}
		return 0;
	}
	
	// 获取计算后的值
	DDWORD DataFormate::GetValue(DDWORD & data1, XJDataType& info1, DDWORD & data2, XJDataType& info2, char opt) {
		DDWORD result;
		memset(&result, 0x00, sizeof(result));
		// 实际使用数据类型
		short cls = info1.Cls >= info2.Cls ? info1.Cls : info2.Cls;
		// 实际使用数据格式
		short type = info1.Type >= info2.Type ? info1.Type : info2.Type;
		switch (cls)
		{
		case Project::DT_Byte:
		{
			if (type == Project::NT_Unsigned) {
				UINT8 ret = { 0 };
				CalValue<UINT8>(data1, info1, data2, info2, &ret, opt, false);
				memcpy(&result, &ret, sizeof(UINT8));
			} else {
				INT8 ret = { 0 };
				CalValue<INT8>(data1, info1, data2, info2, &ret, opt, true);
				memcpy(&result, &ret, sizeof(INT8));
			}
			break;
		}
		case Project::DT_WORD:
		{
			if (type == Project::NT_Unsigned) {
				UINT16 ret = { 0 };
				CalValue<UINT16>(data1, info1, data2, info2, &ret, opt, false);
				memcpy(&result, &ret, sizeof(UINT16));
			} else {
				INT16 ret = { 0 };
				CalValue<INT16>(data1, info1, data2, info2, &ret, opt, true);
				memcpy(&result, &ret, sizeof(INT16));
			}
			break;
		}
		case Project::DT_DWORD:
		{
			if (type == Project::NT_Float) {	// Float
				FLOAT ret = { 0 };
				CalValue<FLOAT>(data1, info1, data2, info2, &ret, opt, true);
				memcpy(&result, &ret, sizeof(FLOAT));
			} else if (type == Project::NT_Unsigned) {
				UINT32 ret = { 0 };
				CalValue<UINT32>(data1, info1, data2, info2, &ret, opt, false);
				memcpy(&result, &ret, sizeof(UINT32));
			} else {
				INT32 ret = { 0 };
				CalValue<INT32>(data1, info1, data2, info2, &ret, opt, true);
				memcpy(&result, &ret, sizeof(INT32));
			}
			break;
		}
		case Project::DT_DDWORD:
		{
			if (type == Project::NT_Float) {	// Double
				DOUBLE ret = { 0 };
				CalValue<DOUBLE>(data1, info1, data2, info2, &ret, opt, true);
				memcpy(&result, &ret, sizeof(DOUBLE));
			} else if (type == Project::NT_Unsigned) {
				UINT64 ret = { 0 };
				CalValue<UINT64>(data1, info1, data2, info2, &ret, opt, false);
				memcpy(&result, &ret, sizeof(UINT64));
			} else {
				INT64 ret = { 0 };
				CalValue<INT64>(data1, info1, data2, info2, &ret, opt, true);
				memcpy(&result, &ret, sizeof(INT64));
			}
			break;
		}
		default:
			break;
		}
		return result;
	}

	// 两数相加
	DDWORD DataFormate::Add(DDWORD& data1, XJDataType& info1, DDWORD& data2, XJDataType& info2)
	{
		DDWORD result = GetValue(data1, info1, data2, info2, '+');
#if 0
		// 实际使用数据格式
		int cls = (info1.Cls >= info2.Cls ? info1.Cls : info2.Cls) - 48;
		int type = info1.Type == '4' || info2.Type == '4' ? Float :
			info1.Type == '3' || info2.Type == '3' ? Unsigned : Signed;
		switch (cls)
		{
		case Byte:
			if (type == Unsigned) {
				UINT8 ret = 0;
				CalValue<UINT8>(data1, info1, data2, info2, &ret, '+', false);
				memcpy(&result, &ret, sizeof(UINT8));
			} else {
				INT8 ret = 0;
				CalValue<INT8>(data1, info1, data2, info2, &ret, '+', true);
				memcpy(&result, &ret, sizeof(INT8));
			}
			break;
		case Word:
			if (type == Unsigned) {
				UINT16 ret = 0;
				CalValue<UINT16>(data1, info1, data2, info2, &ret, '+', false);
				memcpy(&result, &ret, sizeof(UINT16));
			} else {
				INT16 ret = 0;
				CalValue<INT16>(data1, info1, data2, info2, &ret, '+', true);
				memcpy(&result, &ret, sizeof(INT16));
			}
			break;
		case DWord:
			if (type == Float) {
				FLOAT ret = 0;
				CalValue<FLOAT>(data1, info1, data2, info2, &ret, '+', true);
				memcpy(&result, &ret, sizeof(FLOAT));
			} else if (type == Unsigned) {
				UINT32 ret = 0;
				CalValue<UINT32>(data1, info1, data2, info2, &ret, '+', false);
				memcpy(&result, &ret, sizeof(UINT32));
			} else {
				INT32 ret = 0;
				CalValue<INT32>(data1, info1, data2, info2, &ret, '+', true);
				memcpy(&result, &ret, sizeof(INT32));
			}
			break;
		case DDWord:
			if (type == Float) {
				DOUBLE ret = 0;
				CalValue<DOUBLE>(data1, info1, data2, info2, &ret, '+', true);
				memcpy(&result, &ret, sizeof(DOUBLE));
			} else if (type == Unsigned) {
				UINT64 ret = 0;
				CalValue<UINT64>(data1, info1, data2, info2, &ret, '+', false);
				memcpy(&result, &ret, sizeof(UINT64));
			} else {
				INT64 ret = 0;
				CalValue<INT64>(data1, info1, data2, info2, &ret, '+', true);
				memcpy(&result, &ret, sizeof(INT64));
			}
			break;
		default:
			break;
		}
#endif
		return result;
	}

	// 两数相减
	DDWORD DataFormate::Sub(DDWORD& data1, XJDataType& info1, DDWORD& data2, XJDataType& info2)
	{
		DDWORD result = GetValue(data1, info1, data2, info2, '-');
#if 0
		// 实际使用数据格式
		int cls = (info1.Cls >= info2.Cls ? info1.Cls : info2.Cls) - 48;
		int type = info1.Type == '4' || info2.Type == '4' ? Float : 
			info1.Type == '3' || info2.Type == '3' ? Unsigned : Signed;
		switch (cls)
		{
		case Byte:
			if (type == Unsigned) {
				unique_ptr<UINT8> ret = make_unique<UINT8>(0);
				CalValue<UINT8>(data1, info1, data2, info2, ret.get(), '-', false);
				memcpy(&result, ret.get(), sizeof(UINT8));
				ret.reset();
			} else {
				unique_ptr<INT8> ret = make_unique<INT8>(0);
				CalValue<INT8>(data1, info1, data2, info2, ret.get(), '-', true);
				memcpy(&result, ret.get(), sizeof(INT8));
				ret.reset();
			}
			break;
		case Word:
			if (type == Unsigned) {
				unique_ptr<UINT16> ret = make_unique<UINT16>(0);
				CalValue<UINT16>(data1, info1, data2, info2, ret.get(), '-', false);
				memcpy(&result, ret.get(), sizeof(UINT16));
				ret.reset();
			} else {
				unique_ptr<INT16> ret = make_unique<INT16>(0);
				CalValue<INT16>(data1, info1, data2, info2, ret.get(), '-', true);
				memcpy(&result, ret.get(), sizeof(UINT16));
				ret.reset();
			}
			break;
		case DWord:
			if (type == Float) {	// float
				unique_ptr<FLOAT> ret = make_unique<FLOAT>(0);
				CalValue<FLOAT>(data1, info1, data2, info2, ret.get(), '-', true);
				memcpy(&result, ret.get(), sizeof(FLOAT));
				ret.reset();
			} else if (type == Unsigned) {
				unique_ptr<UINT32> ret = make_unique<UINT32>(0);
				CalValue<UINT32>(data1, info1, data2, info2, ret.get(), '-', false);
				memcpy(&result, ret.get(), sizeof(UINT32));
				ret.reset();
			} else {
				unique_ptr<INT32> ret = make_unique<INT32>(0);
				CalValue<INT32>(data1, info1, data2, info2, ret.get(), '-', true);
				memcpy(&result, ret.get(), sizeof(INT32));
				ret.reset();
			}
			break;
		case DDWord:
			if (type == Float) {	// double
				unique_ptr<DOUBLE> ret = make_unique<DOUBLE>(0);
				CalValue<DOUBLE>(data1, info1, data2, info2, ret.get(), '-', true);
				memcpy(&result, ret.get(), sizeof(DOUBLE));
				ret.reset();
			} else if (type == Unsigned) {
				unique_ptr<UINT64> ret = make_unique<UINT64>(0);
				CalValue<UINT64>(data1, info1, data2, info2, ret.get(), '-', false);
				memcpy(&result, ret.get(), sizeof(UINT64));
				ret.reset();
			} else {
				unique_ptr<INT64> ret = make_unique<INT64>(0);
				CalValue<INT64>(data1, info1, data2, info2, ret.get(), '-', true);
				memcpy(&result, ret.get(), sizeof(INT64));
				ret.reset();
			}
			break;
		default:
			break;
		}
#endif
		return result;
	}

	// 两数相乘
	DDWORD DataFormate::Mul(DDWORD& data1, XJDataType& info1, DDWORD& data2, XJDataType& info2)
	{
		DDWORD result = GetValue(data1, info1, data2, info2, '*');
		return result;
	}

	// 两数相除
	DDWORD DataFormate::Div(DDWORD& data1, XJDataType& info1, DDWORD& data2, XJDataType& info2)
	{
		DDWORD result = GetValue(data1, info1, data2, info2, '/');
		return result;
	}

	// 比较数值
	int DataFormate::Cmp(DDWORD& data1, XJDataType& info1, DDWORD& data2, XJDataType& info2)
	{
		// 实际使用数据类型
		short cls = info1.Cls >= info2.Cls ? info1.Cls : info2.Cls;
		// 实际使用数据格式
		short type = info1.Type >= info2.Type ? info1.Type : info2.Type;
		int ret = -1;
		switch (cls)
		{
		case Project::DT_Byte: 
		{
			if (type == Project::NT_Unsigned) {
				UINT8 val1 = GetNumber<UINT8>(data1, info1);
				UINT8 val2 = GetNumber<UINT8>(data2, info2);
				ret = Compare<UINT8>(val1, val2);
			} else {
				INT8 val1 = GetNumber<INT8>(data1, info1);
				INT8 val2 = GetNumber<INT8>(data2, info2);
				ret = Compare<INT8>(val1, val2);
			}
			break;
		}
		case Project::DT_WORD: 
		{
			if (type == Project::NT_Unsigned) {
				UINT16 val1 = GetNumber<UINT16>(data1, info1);
				UINT16 val2 = GetNumber<UINT16>(data2, info2);
				ret = Compare<UINT16>(val1, val2);
			} else {
				INT16 val1 = GetNumber<INT16>(data1, info1);
				INT16 val2 = GetNumber<INT16>(data2, info2);
				ret = Compare<INT16>(val1, val2);
			}
			break;
		}
		case Project::DT_DWORD:
		{
			if (type == Project::NT_Float) {
				FLOAT val1 = GetNumber<FLOAT>(data1, info1);
				FLOAT val2 = GetNumber<FLOAT>(data2, info2);
				ret = Compare<FLOAT>(val1, val2);
			} else if (type == Project::NT_Unsigned) {
				UINT32 val1 = GetNumber<UINT32>(data1, info1);
				UINT32 val2 = GetNumber<UINT32>(data2, info2);
				ret = Compare<UINT32>(val1, val2);
			} else {
				INT32 val1 = GetNumber<INT32>(data1, info1);
				INT32 val2 = GetNumber<INT32>(data2, info2);
				ret = Compare<INT32>(val1, val2);
			}
			break;
		}
		case Project::DT_DDWORD:
		{
			if (type == Project::NT_Float) {
				DOUBLE val1 = GetNumber<DOUBLE>(data1, info1);
				DOUBLE val2 = GetNumber<DOUBLE>(data2, info2);
				ret = Compare<DOUBLE>(val1, val2);
			} else if (type == Project::NT_Unsigned) {
				UINT64 val1 = GetNumber<UINT64>(data1, info1);
				UINT64 val2 = GetNumber<UINT64>(data2, info2);
				ret = Compare<UINT64>(val1, val2);
			} else {
				INT64 val1 = GetNumber<INT64>(data1, info1);
				INT64 val2 = GetNumber<INT64>(data2, info2);
				ret = Compare<INT64>(val1, val2);
			}
			break;
		}
		default:
			break;
		}
		return ret;
	}

	// 返回比较值
	template<typename T>
	int DataFormate::Compare(T & a, T & b) {
		if (a == b) {
			return 0;
		} else if (a < b) {
			return -1;
		} else {
			return 1;
		}
	}

	// 初始化转换比例
	void DataFormate::InitScaleInfo(ScaleInfo& scale, DDWORD& minData, XJDataType& minInfo,
		DDWORD& maxData, XJDataType& maxInfo, int min, int max)
	{
		int ret = Cmp(minData, minInfo, maxData, maxInfo);
		if (ret == 0) {
			scale.A = 0;
			scale.B = 0;
			return;
		}
		DOUBLE minVal = GetNumber<DOUBLE>(minData, minInfo);
		DOUBLE maxVal = GetNumber<DOUBLE>(maxData, maxInfo);
		scale.A = (DOUBLE)(max - min) / (maxVal - minVal);
		scale.B = min - (scale.A * minVal);
	}

	// 获取转换后的值
	int DataFormate::GetScale(ScaleInfo& scale, DDWORD& data, XJDataType& info)
	{
		DOUBLE value = scale.A * GetNumber<DOUBLE>(data, info) + scale.B;
		if (((int)(value * 10) % 10) >= 5)
			value += 1;
		return (int)value;
	}
}