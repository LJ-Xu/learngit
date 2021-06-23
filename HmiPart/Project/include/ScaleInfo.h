#pragma once
#include <vector>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "Point.h"
#include <string>

namespace Project
{
	struct ScaleInfo
	{
		Point Coordinate;
		int Width;
		int Height;
		std::string ScaleContent;					//YÖá±ê×¢ÄÚÈÝ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Coordinate, Width, Height, ScaleContent);
		}

		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<ScaleInfo>& vector, rapidjson::Value& jsonObj);
	};
}

