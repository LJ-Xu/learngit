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
		m_ScaleConfig.X = 50;  //X����
		m_ScaleConfig.Y = 100;  //Y����
		m_ScaleConfig.Width = 200; //���       
		m_ScaleConfig.Height = 100; //�߶�

		m_ScaleConfig.Scale_line.Alpha = 100; //��͸����
		m_ScaleConfig.Scale_line.Color = 0xFF00FF;   //����ɫ
		m_ScaleConfig.Scale_line.Type = 0;     //������      
		m_ScaleConfig.Scale_line.Weight = 3;           //�߿��

		m_ScaleConfig.Scale_style = m_ScaleConfig.Vertical;    //�̶���ʽ

		m_ScaleConfig.Maj_scale = 5;         //���ָ����
		m_ScaleConfig.Second_scale = 1;      //���ָ����

		m_ScaleConfig.Scale_length = 40;     //�̶ȳ���

		m_ScaleConfig.isText = true;        //�Ƿ����ֱ�ע
		m_ScaleConfig.Start_Seg = 1;         //��ʼ�̶���  ��ʶ��Χ
		m_ScaleConfig.End_Seg = 6;           //�����̶���
		m_ScaleConfig.Font_Size = 5;		 //��ע�����С
	}
}