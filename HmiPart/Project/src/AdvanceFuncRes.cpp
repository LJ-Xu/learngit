#include "AdvanceFuncRes.h"
namespace Project
{
	void ExecutionAction::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("UsePopWindow"))
			UsePopWindow = jsonObj["UsePopWindow"].GetBool();
		if (jsonObj.HasMember("WindowNumber"))
			WindowNumber = jsonObj["WindowNumber"].GetInt();
		if (jsonObj.HasMember("UseCallMacros"))
			UseCallMacros = jsonObj["UseCallMacros"].GetBool();
		if (jsonObj.HasMember("MacroName") && !jsonObj["MacroName"].IsNull())
		{
			std::string name = jsonObj["MacroName"].GetString();
			memcpy(MacroName, name.c_str(), name.size());
		}
	}
	/*********************************json½âÎöº¯Êý*******************************************/
	void AdvFuncSetup::CommunicationSetting::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("TimeoutSetting") && jsonObj["TimeoutSetting"].IsObject())
			TimeoutSetting.Parse(jsonObj["TimeoutSetting"]);
		if (jsonObj.HasMember("PopWinTimeoutCounts"))
			PopWinTimeoutCounts = jsonObj["PopWinTimeoutCounts"].GetInt();
		if (jsonObj.HasMember("CallMacroTimeoutCounts"))
			CallMacroTimeoutCounts = jsonObj["CallMacroTimeoutCounts"].GetInt();

		if (jsonObj.HasMember("ErrorSetting") && jsonObj["ErrorSetting"].IsObject())
			ErrorSetting.Parse(jsonObj["ErrorSetting"]);
		if (jsonObj.HasMember("PopWinErrorCounts"))
			PopWinErrorCounts = jsonObj["PopWinErrorCounts"].GetInt();
		if (jsonObj.HasMember("CallMacroErrorCounts"))
			CallMacroErrorCounts = jsonObj["CallMacroErrorCounts"].GetInt();
		
		if (jsonObj.HasMember("HideCell"))
			HideCell = jsonObj["HideCell"].GetBool();
	}
	void AdvFuncSetup::FlashDiskSetting::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("FullMem") && jsonObj["FullMem"].IsObject())
			FullMem.Parse(jsonObj["FullMem"]);
		if (jsonObj.HasMember("TipUpdate"))
			TipUpdate = jsonObj["TipUpdate"].GetBool();
	}
	void AdvFuncSetup::SDCardSetting::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("FullMem") && jsonObj["FullMem"].IsObject())
			FullMem.Parse(jsonObj["FullMem"]);
		if (jsonObj.HasMember("TipUpdate"))
			TipUpdate = jsonObj["TipUpdate"].GetBool();
	}
	void AdvFuncSetup::SystemParam::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("FullMem") && jsonObj["FullMem"].IsObject())
			FullMem.Parse(jsonObj["FullMem"]);
		if (jsonObj.HasMember("CpuOvertop") && jsonObj["CpuOvertop"].IsObject())
			CpuOvertop.Parse(jsonObj["CpuOvertop"]);
	}
	void AdvFuncSetup::Restart::Parse(rapidjson::Value& jsonObj)
	{
		
	}
	void AdvFuncSetup::CallMacros::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("MacroName") && !jsonObj["MacroName"].IsNull())
		{
			std::string name = jsonObj["MacroName"].GetString();
			strcpy(MacroName, name.c_str());
		}
	}
	void AdvFuncSetup::CallWindow::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("CallWindowMode"))
			CallWindowMode = (WinAction)jsonObj["CallWindowMode"].GetInt();
		if (jsonObj.HasMember("WindowNumber"))
			WindowNumber = jsonObj["WindowNumber"].GetInt();
		if (jsonObj.HasMember("IsShowPasswordWindow"))
			IsShowPasswordWindow = jsonObj["IsShowPasswordWindow"].GetBool();
	}
	void AdvFuncSetup::CloseWindow::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("CloseWindowMode"))
			CloseWindowMode = jsonObj["CloseWindowMode"].GetInt();
		if (jsonObj.HasMember("AssignCloseWindowNumber"))
			AssignCloseWindowNumber = jsonObj["AssignCloseWindowNumber"].GetInt();
	}
	void AdvFuncSetup::DataTransfer::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("SendType"))
			SendType = jsonObj["SendType"].GetInt();
		if (jsonObj.HasMember("RegNum"))
			RegNum = jsonObj["RegNum"].GetInt();
		if (jsonObj.HasMember("SrcVarIdRefs") && jsonObj["SrcVarIdRefs"].IsObject())
			SrcVar.Parse(jsonObj["SrcVarIdRefs"]);
		if (jsonObj.HasMember("DstVarIdRefs") && jsonObj["DstVarIdRefs"].IsObject())
			DstVar.Parse(jsonObj["DstVarIdRefs"]);
	}
	void AdvFuncSetup::CallSystemCommand::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("CommandName") && !jsonObj["CommandName"].IsNull())
		{
			std::string name = jsonObj["CommandName"].GetString();
			strcpy(CommandName, name.c_str());
		}
	}
	void AdvanceFuncRes::Parse(std::vector<AdvanceFuncRes>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<AdvanceFuncRes>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			Project::AdvanceFuncRes tmpFunc;
			if (jsonObj[i].HasMember("FunctionName") && !jsonObj[i]["FunctionName"].IsNull())
			{
				tmpFunc.FunctionName = jsonObj[i]["FunctionName"].GetString();
				if (tmpFunc.FunctionName == "CommunicationSetting")
					tmpFunc.FunctionParam.CommunicationSetting.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "FlashDiskSetting")
					tmpFunc.FunctionParam.FlashDiskSetting.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "SDCardSetting")
					tmpFunc.FunctionParam.SDCardSetting.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "SystemParam")
					tmpFunc.FunctionParam.SystemParam.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "Restart")
					tmpFunc.FunctionParam.Restart.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "CallMacros")
					tmpFunc.FunctionParam.CallMacros.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "CallWindow")
					tmpFunc.FunctionParam.CallWindow.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "CloseWindow")
					tmpFunc.FunctionParam.CloseWindow.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "DataTransfer")
					tmpFunc.FunctionParam.DataTransfer.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "CallSystemCommand")
					tmpFunc.FunctionParam.CallSystemCommand.Parse(jsonObj[i]);
			}
			if (jsonObj[i].HasMember("TrigSet") && jsonObj[i]["TrigSet"].IsObject())
			{
				if (jsonObj[i]["TrigSet"].HasMember("Trigmod"))
					tmpFunc.Trigmode = jsonObj[i]["TrigSet"]["Trigmod"].GetInt();
				if (jsonObj[i]["TrigSet"].HasMember("BitTrigCondition"))
					tmpFunc.BitTrigCondition = jsonObj[i]["TrigSet"]["BitTrigCondition"].GetInt();
				if (jsonObj[i]["TrigSet"].HasMember("WordTrigCondition"))
					tmpFunc.WordTrigCondition = (Conditions)jsonObj[i]["TrigSet"]["WordTrigCondition"].GetInt();
				if (jsonObj[i]["TrigSet"].HasMember("WordTrigValue"))
					tmpFunc.WordTrigValue = jsonObj[i]["TrigSet"]["WordTrigValue"].GetDouble();
				if (jsonObj[i]["TrigSet"].HasMember("TrigDatavrIdRef") && jsonObj[i]["TrigSet"]["TrigDatavrIdRef"].IsObject())
					tmpFunc.TrigDatavrIdRef.Parse(jsonObj[i]["TrigSet"]["TrigDatavrIdRef"]);
			}
			
			vector.push_back(tmpFunc);
		}
	}
}
