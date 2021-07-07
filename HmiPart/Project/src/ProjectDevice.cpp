#include "ProjectDevice.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;

namespace Project
{
	void PrjBasicParam::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("OutTime"))
			OutTime = jsonObj["OutTime"].GetInt();
		if (jsonObj.HasMember("RetryTimes"))
			RetryTimes = jsonObj["RetryTimes"].GetInt();
		if (jsonObj.HasMember("DelayTime"))
			DelayTime = jsonObj["DelayTime"].GetInt();
		if (jsonObj.HasMember("PeriodTime"))
			PeriodTime = jsonObj["PeriodTime"].GetInt();
		if (jsonObj.HasMember("MaxReadWords"))
			MaxReadWords = jsonObj["MaxReadWords"].GetInt();
		if (jsonObj.HasMember("MaxWriteWords"))
			MaxWriteWords = jsonObj["MaxWriteWords"].GetInt();
	}

	void PrjNetParam::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Addr") && !jsonObj["Addr"].IsNull())
			Addr = jsonObj["Addr"].GetString();
		if (jsonObj.HasMember("Port"))
			Port = jsonObj["Port"].GetInt();
	}

	void PrjUartParam::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Baud"))
			Baud = jsonObj["Baud"].GetInt();
		if (jsonObj.HasMember("Parity"))
			Parity = jsonObj["Parity"].GetInt();
		if (jsonObj.HasMember("DataBits"))
			DataBits = jsonObj["DataBits"].GetInt();
		if (jsonObj.HasMember("StopBits"))
			StopBits = jsonObj["StopBits"].GetInt();
		
	}
	void InitModbusEndian(HighLowByteSet* param)
	{
		//16位数值
		switch (param->WordEndian)
		{
		case 1:
			//BA	
			param->WordEndian = 0;
			break;
		default:
			//AB		
			param->WordEndian = CHANGE_BYTE;
			break;
		}
		//16位字符
		switch (param->WordStringEndian)
		{
		case 1:
			//BA
			param->WordStringEndian = 0;
			break;
		default:
			//AB		
			param->WordStringEndian = CHANGE_BYTE;
			break;
		}
		//32位数值
		switch (param->DWordEndian)
		{
		case 1:
			//BADC
			param->DWordEndian = 0;
			break;
		case 2:
			//CDAB
			param->DWordEndian = CHANGE_BYTE | CHANGE_WORD;
			break;
		case 3:
			//DCBA
			param->DWordEndian = CHANGE_WORD;
			break;
		default:
			//ABCD
			param->DWordEndian = CHANGE_BYTE;
			break;
		}
		//32位字符
		switch (param->DWordStringEndian)
		{
		case 1:
			//BADC
			param->DWordStringEndian = 0;
			break;
		case 2:
			//CDAB
			param->DWordStringEndian = CHANGE_BYTE | CHANGE_WORD;
			break;
		case 3:
			//DCBA
			param->DWordStringEndian = CHANGE_WORD;
			break;
		default:
			//ABCD
			param->DWordStringEndian = CHANGE_BYTE;
			break;
		}
		//64位数值
		switch (param->DDWordEndian)
		{
		case 1:
			//BADCFEGH
			param->DDWordEndian = 0;
			break;
		case 2:
			//CDABGHEF
			param->DDWordEndian = CHANGE_BYTE | CHANGE_WORD;
			break;
		case 3:
			//DCBAHGFE
			param->DDWordEndian = CHANGE_WORD;
			break;
		case 4:
			//EFGHABCD
			param->DDWordEndian = CHANGE_BYTE | CHANGE_DWORD;
			break;
		case 5:
			//FEHGBADC
			param->DDWordEndian = CHANGE_DWORD;
			break;
		case 6:
			//GHEFCDAB
			param->DDWordEndian = CHANGE_BYTE | CHANGE_WORD | CHANGE_DWORD;
			break;
		case 7:
			//HGFEDCBA
			param->DDWordEndian = CHANGE_WORD | CHANGE_DWORD;
			break;
		default:
			//ABCDEFGH
			param->DDWordEndian = CHANGE_BYTE;
			break;
		}
		//64位字符
		switch (param->DDWordStringEndian)
		{
		case 1:
			//BADCFEGH
			param->DDWordStringEndian = 0;
			break;
		case 2:
			//CDABGHEF
			param->DDWordStringEndian = CHANGE_BYTE | CHANGE_WORD;
			break;
		case 3:
			//DCBAHGFE
			param->DDWordStringEndian = CHANGE_WORD;
			break;
		case 4:
			//EFGHABCD
			param->DDWordStringEndian = CHANGE_BYTE | CHANGE_DWORD;
			break;
		case 5:
			//FEHGBADC
			param->DDWordStringEndian = CHANGE_DWORD;
			break;
		case 6:
			//GHEFCDAB
			param->DDWordStringEndian = CHANGE_BYTE | CHANGE_WORD | CHANGE_DWORD;
			break;
		case 7:
			//HGFEDCBA
			param->DDWordStringEndian = CHANGE_WORD | CHANGE_DWORD;
			break;
		default:
			//ABCDEFGH
			param->DDWordStringEndian = CHANGE_BYTE;
			break;
		}
	}
	void PrjDev::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("DevName") && !jsonObj["DevName"].IsNull())
			DevName = jsonObj["DevName"].GetString();
		if (jsonObj.HasMember("PortID"))
			PortID = jsonObj["PortID"].GetInt();
		if (jsonObj.HasMember("PointStyle"))
			Point = (PointType)jsonObj["PointStyle"].GetInt();
		if (jsonObj.HasMember("StaNo"))
			StaNo = jsonObj["StaNo"].GetInt();
		if (jsonObj.HasMember("ProtocolId") && !jsonObj["ProtocolId"].IsNull())
			ProtocolId = jsonObj["ProtocolId"].GetString();
		if (jsonObj.HasMember("CommParam") && jsonObj["CommParam"].IsObject())
		{
			if (jsonObj["CommParam"].HasMember("BasicParam") && jsonObj["CommParam"]["BasicParam"].IsObject())
				CommParam.BasicParam.Parse(jsonObj["CommParam"]["BasicParam"]);
			if (jsonObj["CommParam"].HasMember("NetParam") && jsonObj["CommParam"]["NetParam"].IsObject())
				CommParam.NetParam.Parse(jsonObj["CommParam"]["NetParam"]);
			if (jsonObj["CommParam"].HasMember("UartParam") && jsonObj["CommParam"]["UartParam"].IsObject())
				CommParam.UartParam.Parse(jsonObj["CommParam"]["UartParam"]);
			if (jsonObj["CommParam"].HasMember("seniorSetPara") && jsonObj["CommParam"]["seniorSetPara"].IsObject())
			{
				if (jsonObj["CommParam"]["seniorSetPara"].HasMember("AdvanceSetUp") &&
					jsonObj["CommParam"]["seniorSetPara"]["AdvanceSetUp"].IsObject())
				{
					Value &advanceset = jsonObj["CommParam"]["seniorSetPara"]["AdvanceSetUp"];
					if (advanceset.HasMember("HighLowByte") && advanceset["HighLowByte"].IsObject())
					{
						if (advanceset["HighLowByte"].HasMember("SixteenInteger"))
							HighLowByte.WordEndian = advanceset["HighLowByte"]["SixteenInteger"].GetInt();
						if (advanceset["HighLowByte"].HasMember("SixteenChars"))
							HighLowByte.WordStringEndian = advanceset["HighLowByte"]["SixteenChars"].GetInt();
						if (advanceset["HighLowByte"].HasMember("ThirtytwoInteger"))
							HighLowByte.DWordEndian = advanceset["HighLowByte"]["ThirtytwoInteger"].GetInt();
						if (advanceset["HighLowByte"].HasMember("ThirtytwoChars"))
							HighLowByte.DWordStringEndian = advanceset["HighLowByte"]["ThirtytwoChars"].GetInt();
						if (advanceset["HighLowByte"].HasMember("SixtyfourInteger"))
							HighLowByte.DDWordEndian = advanceset["HighLowByte"]["SixtyfourInteger"].GetInt();
						if (advanceset["HighLowByte"].HasMember("SixtyfourChars"))
							HighLowByte.DDWordStringEndian = advanceset["HighLowByte"]["SixtyfourChars"].GetInt();
						InitModbusEndian(&HighLowByte);
					}
				}
				if (jsonObj["CommParam"]["seniorSetPara"].HasMember("AdvanceConfig") &&
					!jsonObj["CommParam"]["seniorSetPara"]["AdvanceConfig"].IsNull())
				{
					string param = jsonObj["CommParam"]["seniorSetPara"]["AdvanceConfig"].GetString();
					vector<char>().swap(DevParamData);
					DevParamData.assign(param.begin(), param.end());
				}
			}

		}
	}
	void PrjDev::Parse(std::vector<PrjDev>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<PrjDev>().swap(vector);

		Value::MemberIterator iter = jsonObj.MemberEnd() - 1;
		string maxsize = (iter->name).GetString();
		//int maxsize = atoi(name.c_str()) + 1;
		vector.resize(atoi(maxsize.c_str()) + 1);

		for (rapidjson::Value::MemberIterator iter = jsonObj.MemberBegin();iter != jsonObj.MemberEnd();iter++)
		{
			string name = (iter->name).GetString();
			Value& value = iter->value;
			if (value.IsObject() && !value.IsNull())
			{
				Project::PrjDev res;
				res.Parse(value);
				if ((size_t)atoi(name.c_str()) < vector.size())
					vector[atoi(name.c_str())] = res;
			}
		}
		//std::vector<PrjDev>().swap(vector);
		//for (unsigned i = 0; i < jsonObj.Size(); i++)
		//{
		//	if (!jsonObj[i].IsNull())
		//	{
		//		Project::PrjDev res;
		//		res.Parse(jsonObj[i]);
		//		vector.push_back(res);
		//	}
		//}
	}
	void HMIPortParam::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("netParam") && jsonObj["netParam"].IsObject())
		{
			if (jsonObj["netParam"].HasMember("Addr") && !jsonObj["netParam"]["Addr"].IsNull())
				NetParam.Addr = jsonObj["netParam"]["Addr"].GetString();
			if (jsonObj["netParam"].HasMember("Mask") && !jsonObj["netParam"]["Mask"].IsNull())
				NetParam.Mask = jsonObj["netParam"]["Mask"].GetString();
			if (jsonObj["netParam"].HasMember("GateWay") && !jsonObj["netParam"]["GateWay"].IsNull())
				NetParam.GateWay = jsonObj["netParam"]["GateWay"].GetString();
			if (jsonObj["netParam"].HasMember("IsAuto"))
				NetParam.IsAuto = jsonObj["netParam"]["IsAuto"].GetBool();
		}
		if (jsonObj.HasMember("seriaParam") && jsonObj["seriaParam"].IsObject())
		{
			if (jsonObj["seriaParam"].HasMember("PortName") && !jsonObj["seriaParam"]["PortName"].IsNull())
			{
				string portname = jsonObj["seriaParam"]["PortName"].GetString();
				memcpy(SeriaParam.NamePath, portname.c_str(), portname.size());
			}
			if (jsonObj["seriaParam"].HasMember("Baud"))
				SeriaParam.Baud = jsonObj["seriaParam"]["Baud"].GetInt();
			if (jsonObj["seriaParam"].HasMember("Parity"))
				SeriaParam.Parity = (char)jsonObj["seriaParam"]["Parity"].GetInt();
			if (jsonObj["seriaParam"].HasMember("DataBits"))
				SeriaParam.DataBits = (char)jsonObj["seriaParam"]["DataBits"].GetInt();
			if (jsonObj["seriaParam"].HasMember("StopBits"))
				SeriaParam.StopBits = (char)jsonObj["seriaParam"]["StopBits"].GetInt();
		}
	}
	void HMIPort::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Name") && !jsonObj["Name"].IsNull())
			Name = jsonObj["Name"].GetString();
		if (jsonObj.HasMember("Port"))
			Port = (PortType)jsonObj["Port"].GetInt();
		if (jsonObj.HasMember("Param") && jsonObj["Param"].IsObject())
			Param.Parse(jsonObj["Param"]);
	}

	void HMIPort::Parse(std::vector<HMIPort>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<HMIPort>().swap(vector);

		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::HMIPort res;
				res.Parse(jsonObj[i]);
				vector.push_back(res);
			}
		}
	}
	void ProjectDevice::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			if (json.HasMember("Ports") && json["Ports"].IsArray())
				HMIPort::Parse(Ports, json["Ports"]);
			if (json.HasMember("Devs") && json["Devs"].IsObject())
				PrjDev::Parse(Devs, json["Devs"]);
		}
	}
}