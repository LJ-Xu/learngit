#include "stdafx.h"
#include "GLabelModel.h"
#include <FL/Fl.H>
#include "JsonComm.h"
using namespace rapidjson;
namespace UI
{
	void GLabelModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		m_config.text = "Label:";
		m_config.X = 200;  //X坐标
		m_config.Y = 100;  //Y坐标
		m_config.Width = 80; //宽度
		m_config.Height = 40;//高度
		m_config.backColor = FL_WHITE; //填充颜色
		//m_config.type = FL_FRAME_BOX;
		m_config.type = FL_NO_BOX;
		m_config.font.Name = "song ti";
		m_config.font.Size = 20;
		//m_config.font.color = FL_RED;
		m_config.image.path = "C:/Users/PC/Desktop/IMG_9926.JPG";

	}
}
