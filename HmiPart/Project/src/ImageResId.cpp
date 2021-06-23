#include "ImageResId.h"
namespace Project
{
	void ImageResId::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("KeyVal"))
			if (jsonObj["KeyVal"].IsNull())
				KeyVal = "";
			else
				KeyVal = jsonObj["KeyVal"].GetString();

			
	}
	void ImageResId::Parse(std::vector<ImageResId>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<ImageResId>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::ImageResId* id = new Project::ImageResId();
				id->Parse(jsonObj[i]);
				vector.push_back(*id);
			}

		}
	}
}