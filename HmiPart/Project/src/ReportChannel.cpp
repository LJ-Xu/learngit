#include "ReportChannel.h"
namespace Project
{
	void ReportChannel::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("SimpleGroupName"))
			SimpleGroupName = jsonObj["SimpleGroupName"].GetInt();
		if (jsonObj.HasMember("SimpleGroupNo"))
			SimpleGroupNo = jsonObj["SimpleGroupNo"].GetInt();
		if (jsonObj.HasMember("Channel"))
			Channel = jsonObj["Channel"].GetInt();
		if (jsonObj.HasMember("IntegerNum"))
			IntegerNum = jsonObj["IntegerNum"].GetInt();
		if (jsonObj.HasMember("DecimalNum"))
			DecimalNum = jsonObj["DecimalNum"].GetInt();
		if (jsonObj.HasMember("IsLead"))
			IsLead = jsonObj["IsLead"].GetBool();
		if (jsonObj.HasMember("AlignStyle"))
			AlignStyle = jsonObj["AlignStyle"].GetInt();
		if (jsonObj.HasMember("ViewColor"))
			ViewColor = jsonObj["ViewColor"].GetInt();
	}
	void ReportChannel::Parse(std::vector<ReportChannel>& vector, rapidjson::Value& jsonObj, int simpleGroupName, int simpleGroupNo)
	{
		std::vector<ReportChannel>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::ReportChannel* id = new Project::ReportChannel();
				if ((jsonObj[i].HasMember("SimpleGroupName") && jsonObj[i]["SimpleGroupName"].GetInt() == simpleGroupName)
					&& (jsonObj[i].HasMember("SimpleGroupNo") && jsonObj[i]["SimpleGroupNo"].GetInt() == simpleGroupNo))
				{
					id->Parse(jsonObj[i]);
					vector.push_back(*id);
				}
			}

		}
	}

	void DisplayReportLst::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Project") && !jsonObj["Project"].IsNull())
			ProjectName = jsonObj["Project"].GetString();
		if (jsonObj.HasMember("Describe") && !jsonObj["Describe"].IsNull())
			DescribeName = jsonObj["Describe"].GetString();
		if (jsonObj.HasMember("TitleDescribe") && jsonObj["TitleDescribe"].IsObject())
			TitleDescribe.Parse(jsonObj["TitleDescribe"]);
		if (jsonObj.HasMember("ColSpace"))
			ColWidth = jsonObj["ColSpace"].GetInt();
		if (jsonObj.HasMember("RowSpace"))
			RowHeight = jsonObj["RowSpace"].GetInt();
	}
	void DisplayReportLst::Parse(std::vector<DisplayReportLst>& vector, rapidjson::Value& jsonObj, int simpleGroupName, int simpleGroupNo)
	{
		std::vector<DisplayReportLst>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::DisplayReportLst* id = new Project::DisplayReportLst();
				if ((jsonObj[i].HasMember("SimpleGroupName") && jsonObj[i]["SimpleGroupName"].GetInt() == simpleGroupName)
					&& (jsonObj[i].HasMember("SimpleGroupNo") && jsonObj[i]["SimpleGroupNo"].GetInt() == simpleGroupNo))
				{
					id->Parse(jsonObj[i]);
					vector.push_back(*id);
				}
			}

		}
	}
}