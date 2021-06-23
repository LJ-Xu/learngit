#pragma once
#ifdef WIN32
#include <string>
#include <vector>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "CerealCommon.hpp"
#else
#include "rapidjson/document.h"
#include <string>
#include <vector>
#include "rapidjson/filereadstream.h"
#include "CerealCommon.hpp"
#endif // WIN32
namespace Project
{
	struct ImageResId
	{
		std::string KeyVal = "";
		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<ImageResId>& vector, rapidjson::Value& jsonObj);

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(KeyVal);
		}
	};

}
