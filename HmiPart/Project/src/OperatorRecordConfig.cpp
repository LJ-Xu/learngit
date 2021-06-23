#include "OperatorRecordConfig.h"
namespace Project
{
	void OperationRecordPrint::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("IsEnableRecordSetPrint"))
			IsEnableRecordSetPrint = jsonObj["IsEnableRecordSetPrint"].GetBool();
		if (jsonObj.HasMember("PrintDirection"))
			PrintDirection = jsonObj["PrintDirection"].GetInt();
		if (jsonObj.HasMember("PrintFontStyle") && jsonObj["PrintFontStyle"].IsObject())
			PrintFontStyle.Parse(jsonObj["PrintFontStyle"]);
		if (jsonObj.HasMember("PrintRange"))
			PrintRange = jsonObj["PrintRange"].GetInt();
		if (jsonObj.HasMember("PrintStartTime"))
			PrintStartTime = jsonObj["PrintStartTime"].GetInt64();
		if (jsonObj.HasMember("PrintEndTime"))
			PrintEndTime = jsonObj["PrintEndTime"].GetInt64();
		if (jsonObj.HasMember("PrintCount"))
			PrintCount = jsonObj["PrintCount"].GetInt();
		if (jsonObj.HasMember("ControlAddress") && jsonObj["ControlAddress"].IsObject())
			ControlAddress.Parse(jsonObj["ControlAddress"]);
		if (jsonObj.HasMember("PrintCond"))
			PrintCond = jsonObj["PrintCond"].GetInt();
		if (jsonObj.HasMember("IsDisplayPrintDate"))
			IsDisplayPrintDate = jsonObj["IsDisplayPrintDate"].GetBool();
		if (jsonObj.HasMember("PrintDateFormate"))
			PrintDateFormate = (DateMode)jsonObj["PrintDateFormate"].GetInt();
		if (jsonObj.HasMember("PrintTimeFormate"))
			PrintTimeFormate = (TimeMode)jsonObj["PrintTimeFormate"].GetInt();
		if (jsonObj.HasMember("Scaling"))
			Scaling = jsonObj["Scaling"].GetDouble();
		if (jsonObj.HasMember("PrintItems") && jsonObj["PrintItems"].IsArray())
		{
			bool select;
			std::vector<string>().swap(PrintItems);
			for (unsigned i = 0; i < jsonObj["PrintItems"].Size(); i++)
			{
				if (jsonObj["PrintItems"][i].HasMember("IsSelect"))
					select = jsonObj["PrintItems"][i]["IsSelect"].GetBool();
				if (select && jsonObj["PrintItems"][i].HasMember("Describe") &&
					!jsonObj["PrintItems"][i]["Describe"].IsNull())
					PrintItems.push_back(jsonObj["PrintItems"][i]["Describe"].GetString());
			}
		}
		if (jsonObj.HasMember("PrintSortMode"))
			PrintSortMode = jsonObj["PrintSortMode"].GetInt();
		if (jsonObj.HasMember("PrintItemDateFormate"))
			PrintItemDateFormate = (DateMode)jsonObj["PrintItemDateFormate"].GetInt();
		if (jsonObj.HasMember("PrintItemTimeFormate"))
			PrintItemTimeFormate = (TimeMode)jsonObj["PrintItemTimeFormate"].GetInt();
	}
}