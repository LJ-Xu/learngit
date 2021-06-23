#include "XYLineChannel.h"
namespace Project
{
	void XYLineChannel::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Index"))
			Index = jsonObj["Index"].GetInt();
		if (jsonObj.HasMember("IsXYDataFromSameSource"))
			IsXYDataFromSameSource = jsonObj["IsXYDataFromSameSource"].GetBool();
		if (jsonObj.HasMember("XAdd") && jsonObj["XAdd"].IsObject())
			StartXAddr.Parse(jsonObj["XAdd"]);
		if (jsonObj.HasMember("YAdd") && jsonObj["YAdd"].IsObject())
			StartYAddr.Parse(jsonObj["YAdd"]);
		if (jsonObj.HasMember("CurveStyle"))
			CurveStyle = (XYLineMode)jsonObj["CurveStyle"].GetInt();
		if (jsonObj.HasMember("LineColor"))
			LineColor = jsonObj["LineColor"].GetInt();
		if (jsonObj.HasMember("LineSize"))
			LineSize = jsonObj["LineSize"].GetInt();
		if (jsonObj.HasMember("LineStyle"))
			LineStyle = jsonObj["LineStyle"].GetInt();
		if (jsonObj.HasMember("DrawDotColor"))
			DrawDotColor = jsonObj["DrawDotColor"].GetInt();
		if (jsonObj.HasMember("DrawDotSize"))
			DrawDotSize = jsonObj["DrawDotSize"].GetInt();
		if (jsonObj.HasMember("DrawDotStyle"))
			DrawDotStyle = (DotStyle)jsonObj["DrawDotStyle"].GetInt();
	}
	void XYLineChannel::Parse(std::vector<XYLineChannel>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<XYLineChannel>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::XYLineChannel res;
				res.Parse(jsonObj[i]);
				vector.push_back(res);
			}
		}
	}
}
