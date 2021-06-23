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

		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}
		MotorConfig.InitBaseData(json);
		////对象（地址数据）

		//数据
		if (json.HasMember("DataCtlSign") && json["DataCtlSign"].IsObject())
			MotorConfig.DataCtlSign.Parse(json["DataCtlSign"]);


		//逻辑：控制逻辑 0 为正逻辑 1为负逻辑
		if (json.HasMember("CtlLogic"))
			MotorConfig.CtlLogic = json["CtlLogic"].GetInt();


		//最大值是否由寄存器指定 
		if (json.HasMember("UseBlink"))
			MotorConfig.UseBlink = json["UseBlink"].GetBool();
		 

		//闪烁状态:0为On状态闪烁，1为Off状态闪烁
		if (json.HasMember("BlinkState"))
			MotorConfig.BlinkState = json["BlinkState"].GetInt();
		//闪烁频率
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
		//0为on闪烁
		MotorConfig.BlinkState = 0;
		//1秒5次
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