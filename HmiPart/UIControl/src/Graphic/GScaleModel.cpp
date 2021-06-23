#include "stdafx.h"
#include "GScaleModel.h"

#include "JsonComm.h"
using namespace rapidjson;

namespace UI
{
	void GScaleModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		m_ScaleConfig.X = 50;  //X坐标
		m_ScaleConfig.Y = 100;  //Y坐标
		m_ScaleConfig.Width = 200; //宽度       
		m_ScaleConfig.Height = 100; //高度

		m_ScaleConfig.Scale_line.Alpha = 100; //线透明度
		m_ScaleConfig.Scale_line.Color = 0xFF00FF;   //线颜色
		m_ScaleConfig.Scale_line.Type = 0;     //线类型      
		m_ScaleConfig.Scale_line.Weight = 3;           //线宽度

		m_ScaleConfig.Scale_style = m_ScaleConfig.Vertical;    //刻度样式

		m_ScaleConfig.Maj_scale = 5;         //主分割段数
		m_ScaleConfig.Second_scale = 1;      //副分割段数

		m_ScaleConfig.Scale_length = 40;     //刻度长度

		m_ScaleConfig.isText = true;        //是否文字标注
		m_ScaleConfig.Start_Seg = 1;         //起始刻度数  标识范围
		m_ScaleConfig.End_Seg = 6;           //结束刻度数
		m_ScaleConfig.Font_Size = 5;		 //标注字体大小
	}
}