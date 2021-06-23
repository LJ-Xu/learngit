#pragma once
#include <string>
#include <vector>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "StatusRes.h"
namespace Project
{
	struct DisplayReportLst
	{
		std::string ProjectName;			//显示项目
		std::string DescribeName;			//项目表述
		StatusRes TitleDescribe;			//多语言项目描述
		int RowHeight;						//行高
		int ColWidth;						//列宽

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ProjectName, TitleDescribe, RowHeight, ColWidth);
		}

		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<DisplayReportLst>& vector, rapidjson::Value& jsonObj,
			int simpleGroupName, int simpleGroupNo);
	};
	class ReportChannel
	{
	public:
		int SimpleGroupName;		//采样组
		int SimpleGroupNo;			//采样编号
		int Channel;				//采样通道
		int IntegerNum;				//整数位
		int DecimalNum;				//小数位
		bool IsLead;				//是否使用前导0
		int AlignStyle;				//对齐方式
		int ViewColor;				//字体颜色

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(SimpleGroupName, SimpleGroupNo, Channel, IntegerNum, 
				DecimalNum, IsLead, AlignStyle, ViewColor);
		}

		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<ReportChannel>& vector, rapidjson::Value& jsonObj,
			int simpleGroupName, int simpleGroupNo);
	};
}

