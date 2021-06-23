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
		std::string ProjectName;			//��ʾ��Ŀ
		std::string DescribeName;			//��Ŀ����
		StatusRes TitleDescribe;			//��������Ŀ����
		int RowHeight;						//�и�
		int ColWidth;						//�п�

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
		int SimpleGroupName;		//������
		int SimpleGroupNo;			//�������
		int Channel;				//����ͨ��
		int IntegerNum;				//����λ
		int DecimalNum;				//С��λ
		bool IsLead;				//�Ƿ�ʹ��ǰ��0
		int AlignStyle;				//���뷽ʽ
		int ViewColor;				//������ɫ

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

