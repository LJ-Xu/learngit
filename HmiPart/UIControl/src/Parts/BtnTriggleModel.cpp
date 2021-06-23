#include "stdafx.h"
#include "BtnTriggleModel.h"
#include <FL/Fl.H>
#include "JsonComm.h"
using namespace rapidjson;
namespace UI
{
	void BtnTriggleModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		m_config.text = "button:";
		m_config.X = 100;  //X����
		m_config.Y = 100;  //Y����
		m_config.Width = 80; //���
		m_config.Height = 40;//�߶�
		m_config.backColor = FL_WHITE;
//		m_config.font.color = FL_GREEN;

		m_config.image.path = "C:/Users/PC/Desktop/IMG_9926.JPG";
		m_config.font.Name = "����";

	}
}