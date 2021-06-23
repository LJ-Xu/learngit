#include "DataTableUnit.h"
namespace Project
{
	void DataCell::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("FillType"))
			CellType = jsonObj["FillType"].GetInt();
		if (jsonObj.HasMember("Text") && !jsonObj["Text"].IsNull())
			Text = jsonObj["Text"].GetString();
		if (jsonObj.HasMember("Editable"))
			IsEditable = jsonObj["Editable"].GetBool();
		if (jsonObj.HasMember("AddrType"))
			AddrType = jsonObj["AddrType"].GetInt();
		if (jsonObj.HasMember("VarIdRef") && jsonObj["VarIdRef"].IsObject())
			DataAddrVar.Parse(jsonObj["VarIdRef"]);
		if (jsonObj.HasMember("IntegerNum"))
			IntegerNum = jsonObj["IntegerNum"].GetInt();
		if (jsonObj.HasMember("DecimalNum"))
			DecimalNum = jsonObj["DecimalNum"].GetInt();
		if (jsonObj.HasMember("IsFillZero"))
			IsFillZero = jsonObj["IsFillZero"].GetBool();
		if (jsonObj.HasMember("RegNum"))
			RegCount = jsonObj["RegNum"].GetInt();
		if (jsonObj.HasMember("Encoding"))
			Rule = jsonObj["Encoding"].GetInt();
	}

	void DataCell::Parse(std::vector<DataCell>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<DataCell>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::DataCell* id = new Project::DataCell();
				id->IsNull = false;
				id->Parse(jsonObj[i]);
				vector.push_back(*id);
			}
			else
			{
				Project::DataCell* id = new Project::DataCell();
				id->IsNull = true;
				vector.push_back(*id);
			}

		}
	}
}