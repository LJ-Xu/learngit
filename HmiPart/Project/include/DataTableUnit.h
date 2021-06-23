#pragma once
#include "MoveableDisplayUnit.h"
#include "ImageResId.h"
#include "BaseLine.h"
//#include "DataInfo.h"
namespace Project
{
	//enum DataCodeRule {
	//	DataASCII,
	//	DataUTF_8,
	//	DataUTF_16,
	//	DataGB2312
	//};
	struct DataCell
	{
		int CellType;						//填充类型 0:地址监控 1:文本
		string Text;						//文本字符串

		bool IsEditable;					//是否可编辑
		int AddrType;						//地址类型 0:数值	1:字符串

		DataVarId DataAddrVar;				//读取寄存器地址
		bool IsFillZero;					//是否使用前导0
		int IntegerNum;
		int DecimalNum;

		int RegCount;						//寄存器数
		int Rule;							//编码格式

		bool IsNull;						//是否为空
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(CellType, Text, IsEditable, AddrType, IsNull,
				DataAddrVar, IntegerNum, IntegerNum, DecimalNum, RegCount, Rule);
		}

		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<DataCell>& vector, rapidjson::Value& jsonObj);
	};
	class DataTableUnit :public MoveableDisplayUnit
	{
	public:
		int Appearance;				//外观样式 0：使用图片样式 1：使用自定义外观
		ImageResId Key;				//外观图片

		int TableBgColor;			//列表背景颜色
		int TitleBgColor;			//标题背景颜色
		bool IsUseBorder;			//是否使用外框
		BaseLine TableBorderStyle;	//外框线样式
		BaseLine GridBorderStyle;	//网格线样式
		bool IsRowDividLine;		//是否使用行网格线
		bool IsColDividLine;		//是否使用列网格线

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Appearance, Key, TableBgColor, TitleBgColor, IsUseBorder,
				TableBorderStyle, GridBorderStyle, IsRowDividLine, IsColDividLine );
		}
	};

}
