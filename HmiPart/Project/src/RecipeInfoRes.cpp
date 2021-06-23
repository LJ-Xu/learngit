#include "RecipeInfoRes.h"
namespace Project
{
	void RecipeInfoRes::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Tag") && !jsonObj["Tag"].IsNull())
			ColTitle = jsonObj["Tag"].GetString();
		if (jsonObj.HasMember("DataType"))
			DataType = (VarDataType)jsonObj["DataType"].GetInt();
		if (jsonObj.HasMember("DataFmt"))
			DataFmt = (VarNumberType)jsonObj["DataFmt"].GetInt();
		if (jsonObj.HasMember("IntegerNum"))
			IntegerNum = jsonObj["IntegerNum"].GetInt();
		if (jsonObj.HasMember("DecimalNum"))
			DecimalNum = jsonObj["DecimalNum"].GetInt();
		if (jsonObj.HasMember("UsedNum"))
			RegCount = jsonObj["UsedNum"].GetInt();
		if (jsonObj.HasMember("Editable"))
			Editable = jsonObj["Editable"].GetBool();
		//if (jsonObj.HasMember("RecipeData") && jsonObj["RecipeData"].IsArray())
		//{
		//	std::vector<std::string>().swap(RecipeData);
		//	for (size_t i = 0; i < jsonObj["RecipeData"].Size(); i++)
		//	{
		//		if(!jsonObj["RecipeData"][i].IsNull())
		//			RecipeData.push_back(jsonObj["RecipeData"][i].GetString());
		//	}
		//}
	}

	void RecipeInfoRes::Parse(std::vector<RecipeInfoRes>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<RecipeInfoRes>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::RecipeInfoRes res;
				res.Parse(jsonObj[i]);
				vector.push_back(res);
			}
		}
	}
}