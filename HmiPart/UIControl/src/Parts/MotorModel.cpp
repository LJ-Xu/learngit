#include "stdafx.h"
#include "MotorModel.h"
#include <FL/Enumerations.H>

#include "JsonComm.h"
using namespace rapidjson;
namespace UI
{
	void MotorModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());

		if (json.HasParseError())	//�жϽ����Ƿ���ȷ
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}
		MotorConfig.InitBaseData(json);
		////���󣨵�ַ���ݣ�

		//����
		if (json.HasMember("DataCtlSign") && json["DataCtlSign"].IsObject())
			MotorConfig.DataCtlSign.Parse(json["DataCtlSign"]);


		//�߼��������߼� 0 Ϊ���߼� 1Ϊ���߼�
		if (json.HasMember("CtlLogic"))
			MotorConfig.CtlLogic = json["CtlLogic"].GetInt();


		//���ֵ�Ƿ��ɼĴ���ָ�� 
		if (json.HasMember("UseBlink"))
			MotorConfig.UseBlink = json["UseBlink"].GetBool();
		 

		//��˸״̬:0ΪOn״̬��˸��1ΪOff״̬��˸
		if (json.HasMember("BlinkState"))
			MotorConfig.BlinkState = json["BlinkState"].GetInt();
		//��˸Ƶ��
		if (json.HasMember("BlinkFrequency"))
		{
			MotorConfig.BlinkFrequency = json["BlinkFrequency"].GetFloat();
			MotorConfig.BlinkFrequency = MotorConfig.BlinkFrequency <= 0 ? 1 : MotorConfig.BlinkFrequency;
		}
			
		if (json.HasMember("KeyImgs"))
		{
			if (json["KeyImgs"].IsArray())
			{
				MotorConfig.KeyImgOn.Parse(json["KeyImgs"][(1+ MotorConfig.CtlLogic)%2]);
				MotorConfig.KeyImgOff.Parse(json["KeyImgs"][(0 + MotorConfig.CtlLogic) % 2]);
			}
		}

		//if (json.HasMember("KeyImgOn"))
		//	MotorConfig.KeyImgOn.Parse(json["KeyImgOn"]);

		//if (json.HasMember("KeyImgOff"))
		//	MotorConfig.KeyImgOff.Parse(json["KeyImgOff"]);
		//m_motorConfig.testMode = false;
	}
	void MotorModel::InitTestData()
	{
		MotorConfig.X = 50;
		MotorConfig.Y = 50;
		MotorConfig.Width = 100;
		MotorConfig.Height = 100;
		MotorConfig.IsEnable = false;
		MotorConfig.CtrlName = "MOTRO0";
		//m_motorConfig.DataCtlSign.Vid = 0;
		
		MotorConfig.CtlLogic = 0;
		MotorConfig.UseBlink = true;
		//0Ϊon��˸
		MotorConfig.BlinkState = 0;
		//1��5��
		//m_motorConfig.BlinkFrequency = 0.2;
		//m_motorConfig.testMode = true;
		//m_motorConfig.KeyImgOn.Parse(json["KeyImgOn"]);
		//m_motorConfig.KeyImgOff.Parse(json["KeyImgOff"]);

		
	}
	void * MotorModel::GetTestConfig()
	{

		return &MotorConfig;
	}
}