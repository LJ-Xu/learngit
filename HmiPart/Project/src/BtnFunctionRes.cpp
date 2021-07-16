#include "BtnFunctionRes.h"
namespace Project
{

	/*********************************json½âÎöº¯Êý*******************************************/
	void FunctionSetup::SetCoil::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Opt"))
			ActionCoil = (CoilAction)jsonObj["Opt"].GetInt();
		if (jsonObj.HasMember("WriteVarIdRef") && jsonObj["WriteVarIdRef"].IsObject())
			RegVar.Parse(jsonObj["WriteVarIdRef"]);
	}
	void FunctionSetup::SetData::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Opt"))
			ActionData = (DataAction)jsonObj["Opt"].GetInt();
		if (jsonObj.HasMember("Value") && jsonObj["Value"].IsObject())
		{
			if (jsonObj["Value"].HasMember("Num"))
				Value = jsonObj["Value"]["Num"].GetDouble();
			if (jsonObj["Value"].HasMember("VarIdRef") && jsonObj["Value"]["VarIdRef"].IsObject())
				ValueVar.Parse(jsonObj["Value"]["VarIdRef"]);
		}
		if (jsonObj.HasMember("WriteVarIdRef") && jsonObj["WriteVarIdRef"].IsObject())
			WriteVarIdRef.Parse(jsonObj["WriteVarIdRef"]);
		if (jsonObj.HasMember("Increase") && jsonObj["Increase"].IsObject())
		{
			if (jsonObj["Increase"].HasMember("Num"))
				IncreaseValue = jsonObj["Increase"]["Num"].GetDouble();
			if (jsonObj["Increase"].HasMember("VarIdRef") && jsonObj["Increase"]["VarIdRef"].IsObject())
				IncreaseValueVar.Parse(jsonObj["Increase"]["VarIdRef"]);
		}
		if (jsonObj.HasMember("UpperLmt") && jsonObj["UpperLmt"].IsObject())
		{
			if (jsonObj["UpperLmt"].HasMember("Num"))
				UpperLmtValue = jsonObj["UpperLmt"]["Num"].GetDouble();
			if (jsonObj["UpperLmt"].HasMember("VarIdRef") && jsonObj["UpperLmt"]["VarIdRef"].IsObject())
				UpperLmtValueVar.Parse(jsonObj["UpperLmt"]["VarIdRef"]);
		}
		if (jsonObj.HasMember("LowerLmt") && jsonObj["LowerLmt"].IsObject())
		{
			if (jsonObj["LowerLmt"].HasMember("Num"))
				LowerLmtValue = jsonObj["LowerLmt"]["Num"].GetDouble();
			if (jsonObj["LowerLmt"].HasMember("VarIdRef") && jsonObj["LowerLmt"]["VarIdRef"].IsObject())
				LowerLmtValueVar.Parse(jsonObj["LowerLmt"]["VarIdRef"]);
		}
		if (jsonObj.HasMember("Loop"))
			Loop = jsonObj["Loop"].GetBool();
		if (jsonObj.HasMember("Rate"))
			Rate = jsonObj["Rate"].GetFloat();
	}
	void FunctionSetup::Arithmetic::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Opt"))
			ActionMath = (MathAction)jsonObj["Opt"].GetInt();
		if (jsonObj.HasMember("LeftVal") && jsonObj["LeftVal"].IsObject())
		{
			if (jsonObj["LeftVal"].HasMember("Type"))
				Left.Type = (MathDataType)jsonObj["LeftVal"]["Type"].GetInt();
			if (jsonObj["LeftVal"].HasMember("Value") && jsonObj["LeftVal"]["Value"].IsObject())
			{
				if(jsonObj["LeftVal"]["Value"].HasMember("Num"))
					Left.Value = jsonObj["LeftVal"]["Value"]["Num"].GetDouble();
				if (jsonObj["LeftVal"]["Value"].HasMember("VarIdRef") && 
					jsonObj["LeftVal"]["Value"]["VarIdRef"].IsObject())
					Left.ValueVar.Parse(jsonObj["LeftVal"]["Value"]["VarIdRef"]);
			}
		}
		if (jsonObj.HasMember("RightVal") && jsonObj["RightVal"].IsObject())
		{
			if (jsonObj["RightVal"].HasMember("Type"))
				Right.Type = (MathDataType)jsonObj["RightVal"]["Type"].GetInt();
			if (jsonObj["RightVal"].HasMember("Value") && jsonObj["RightVal"]["Value"].IsObject())
			{
				if (jsonObj["RightVal"]["Value"].HasMember("Num"))
					Right.Value = jsonObj["RightVal"]["Value"]["Num"].GetDouble();
				if (jsonObj["RightVal"]["Value"].HasMember("VarIdRef") &&
					jsonObj["RightVal"]["Value"]["VarIdRef"].IsObject())
					Right.ValueVar.Parse(jsonObj["RightVal"]["Value"]["VarIdRef"]);
			}
		}
		if (jsonObj.HasMember("WriteVarRef") && jsonObj["WriteVarRef"].IsObject())
			WriteVarRef.Parse(jsonObj["WriteVarRef"]);
		if (jsonObj.HasMember("LowerUsed"))
			LowerUsed = jsonObj["LowerUsed"].GetBool();
		if (jsonObj.HasMember("LowerLmt") && jsonObj["LowerLmt"].IsObject())
		{
			if (jsonObj["LowerLmt"].HasMember("Num"))
				LowerLmtValue = jsonObj["LowerLmt"]["Num"].GetDouble();
			if(jsonObj["LowerLmt"].HasMember("VarIdRef") && jsonObj["LowerLmt"]["VarIdRef"].IsObject())
				LowerLmtValueVar.Parse(jsonObj["LowerLmt"]["VarIdRef"]);
		}
		if (jsonObj.HasMember("UpperUsed"))
			UpperUsed = jsonObj["UpperUsed"].GetBool();
		if (jsonObj.HasMember("UpperLmt") && jsonObj["UpperLmt"].IsObject())
		{
			if (jsonObj["UpperLmt"].HasMember("Num"))
				UpperLmtValue = jsonObj["UpperLmt"]["Num"].GetDouble();
			if (jsonObj["UpperLmt"].HasMember("VarIdRef") && jsonObj["UpperLmt"]["VarIdRef"].IsObject())
				UpperLmtValueVar.Parse(jsonObj["UpperLmt"]["VarIdRef"]);
		}
	}
	void FunctionSetup::SendData::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("SendType"))
			SendType = jsonObj["SendType"].GetInt();
		if (jsonObj.HasMember("RegNum"))
			RegNum = jsonObj["RegNum"].GetInt();
		if (jsonObj.HasMember("SrcVarIdRef") && jsonObj["SrcVarIdRef"].IsObject())
			SrcVar.Parse(jsonObj["SrcVarIdRef"]);
		if (jsonObj.HasMember("DstVarIdRef") && jsonObj["DstVarIdRef"].IsObject())
			DstVar.Parse(jsonObj["DstVarIdRef"]);
	}
	void FunctionSetup::SwitchScreen::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Action"))
			Frame = (FrameAction)jsonObj["Action"].GetInt();
		if (jsonObj.HasMember("WinNo"))
			FrameNum = jsonObj["WinNo"].GetInt();
		if (jsonObj.HasMember("PswUsed"))
			UsePasswd = jsonObj["PswUsed"].GetBool();
	}
	void FunctionSetup::OpenWin::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Action"))
			ActionWin = (WinAction)jsonObj["Action"].GetInt();
		if (jsonObj.HasMember("ExchangeWinNo"))
			ExchangeWinNo = jsonObj["ExchangeWinNo"].GetInt();
		if (jsonObj.HasMember("PopUpWinNo"))
			PopUpWinNo = jsonObj["PopUpWinNo"].GetInt();
		if (jsonObj.HasMember("PswUsed"))
			UsePasswd = jsonObj["PswUsed"].GetBool();
	}
	void FunctionSetup::CloseWin::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Action"))
			CloseAction = (WinAction)jsonObj["Action"].GetInt();
		if (jsonObj.HasMember("WinNo"))
			CloseNum = jsonObj["WinNo"].GetInt();
		if (jsonObj.HasMember("PswUsed"))
			UsePasswd = jsonObj["PswUsed"].GetBool();
	}
	void FunctionSetup::ImportCSVdata::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("OrgVarIdRef") && jsonObj["OrgVarIdRef"].IsObject())
			OrgVarIdRef.Parse(jsonObj["OrgVarIdRef"]);
		if (jsonObj.HasMember("FileAddr") && jsonObj["FileAddr"].IsObject())
		{
			if (jsonObj["FileAddr"].HasMember("Pos"))
				FileAddr = (FlieLocation)jsonObj["FileAddr"]["Pos"].GetInt();
			if (jsonObj["FileAddr"].HasMember("PosVar") && jsonObj["FileAddr"]["PosVar"].IsObject())
				LocationVar.Parse(jsonObj["FileAddr"]["PosVar"]);
		}
		if (jsonObj.HasMember("NameType") && jsonObj["NameType"].IsObject())
		{
			if (jsonObj["NameType"].HasMember("Name") && !jsonObj["NameType"]["Name"].IsNull())
			{
				std::string name = jsonObj["NameType"]["Name"].GetString();
				memset(FileName,'\0',128);
				memcpy(FileName, name.c_str(), name.size());
			}
			if (jsonObj["NameType"].HasMember("Style"))
				NameType = (FlieNameType)jsonObj["NameType"]["Style"].GetInt();
			if (jsonObj["NameType"].HasMember("NameVar") && jsonObj["NameType"]["NameVar"].IsObject())
				NameVar.Parse(jsonObj["NameType"]["NameVar"]);
		}
		
		if (jsonObj.HasMember("DataSize"))
			Count = jsonObj["DataSize"].GetInt();
		if (jsonObj.HasMember("DataContents") && jsonObj["DataContents"].IsArray())
		{
			//std::vector<ImportCSVdata::DataContent>().swap(Data);
			DataSize = jsonObj["DataContents"].Size();
			for (size_t i = 0; i < jsonObj["DataContents"].Size(); i++)
			{
				if (i >= 100)
					return;
				DataContent content;
				if (jsonObj["DataContents"][i].HasMember("Id"))
					content.Id = jsonObj["DataContents"][i]["Id"].GetInt();
				if (jsonObj["DataContents"][i].HasMember("Title") && !jsonObj["DataContents"][i]["Title"].IsNull())
				{
					//content.Title = jsonObj["Data"][i]["Title"].GetString();
					std::string title = jsonObj["DataContents"][i]["Title"].GetString();
					memcpy(content.Title, title.c_str(), title.size());
				}
				if (jsonObj["DataContents"][i].HasMember("DataType"))
					content.DataType = jsonObj["DataContents"][i]["DataType"].GetInt();
				if (jsonObj["DataContents"][i].HasMember("DataFmt"))
					content.DataFmt = jsonObj["DataContents"][i]["DataFmt"].GetInt();
				if (jsonObj["DataContents"][i].HasMember("Number"))
					content.Number = jsonObj["DataContents"][i]["Number"].GetInt();
				if (jsonObj["DataContents"][i].HasMember("IntegerNum"))
					content.IntegerNum = jsonObj["DataContents"][i]["IntegerNum"].GetInt();
				if (jsonObj["DataContents"][i].HasMember("DecimalNum"))
					content.DecimalNum = jsonObj["DataContents"][i]["DecimalNum"].GetInt();
				Data[i] = content;
			}
		}
		if (jsonObj.HasMember("StatusVarRef") && jsonObj["StatusVarRef"].IsObject())
			StatusVarRef.Parse(jsonObj["StatusVarRef"]);
		if (jsonObj.HasMember("ResultVarRef") && jsonObj["ResultVarRef"].IsObject())
			ResultVarRef.Parse(jsonObj["ResultVarRef"]);
		if (jsonObj.HasMember("ProgressVarRef") && jsonObj["ProgressVarRef"].IsObject())
			ProgressVarRef.Parse(jsonObj["ProgressVarRef"]);
	}
	void FunctionSetup::ExportCSVdata::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("OrgVarIdRef") && jsonObj["OrgVarIdRef"].IsObject())
			OrgVarIdRef.Parse(jsonObj["OrgVarIdRef"]);
		if (jsonObj.HasMember("FileAddr") && jsonObj["FileAddr"].IsObject())
		{
			if (jsonObj["FileAddr"].HasMember("Pos"))
				FileAddr = (FlieLocation)jsonObj["FileAddr"]["Pos"].GetInt();
			if (jsonObj["FileAddr"].HasMember("PosVar") && jsonObj["FileAddr"]["PosVar"].IsObject())
				LocationVar.Parse(jsonObj["FileAddr"]["PosVar"]);
		}
		if (jsonObj.HasMember("NameType") && jsonObj["NameType"].IsObject())
		{
			if (jsonObj["NameType"].HasMember("Name") && !jsonObj["NameType"]["Name"].IsNull())
			{
				std::string name = jsonObj["NameType"]["Name"].GetString();
				memset(FileName, '\0', 128);
				memcpy(FileName, name.c_str(), name.size());
			}
			if (jsonObj["NameType"].HasMember("Style"))
				NameType = (FlieNameType)jsonObj["NameType"]["Style"].GetInt();
			if (jsonObj["NameType"].HasMember("NameVar") && jsonObj["NameType"]["NameVar"].IsObject())
				NameVar.Parse(jsonObj["NameType"]["NameVar"]);
		}

		if (jsonObj.HasMember("DataSize"))
			Count = jsonObj["DataSize"].GetInt();
		if (jsonObj.HasMember("DataContents") && jsonObj["DataContents"].IsArray())
		{
			DataSize = jsonObj["DataContents"].Size();
			for (size_t i = 0; i < jsonObj["DataContents"].Size(); i++)
			{
				if (i >= 100)
					return;
				DataContent content;
				if (jsonObj["DataContents"][i].HasMember("Id"))
					content.Id = jsonObj["DataContents"][i]["Id"].GetInt();
				if (jsonObj["DataContents"][i].HasMember("Title") && !jsonObj["DataContents"][i]["Title"].IsNull())
				{
					//content.Title = jsonObj["Data"][i]["Title"].GetString();
					std::string title = jsonObj["DataContents"][i]["Title"].GetString();
					memcpy(content.Title, title.c_str(), title.size());
				}
				if (jsonObj["DataContents"][i].HasMember("DataType"))
					content.DataType = jsonObj["DataContents"][i]["DataType"].GetInt();
				if (jsonObj["DataContents"][i].HasMember("DataFmt"))
					content.DataFmt = jsonObj["DataContents"][i]["DataFmt"].GetInt();
				if (jsonObj["DataContents"][i].HasMember("Number"))
					content.Number = jsonObj["DataContents"][i]["Number"].GetInt();
				if (jsonObj["DataContents"][i].HasMember("IntegerNum"))
					content.IntegerNum = jsonObj["DataContents"][i]["IntegerNum"].GetInt();
				if (jsonObj["DataContents"][i].HasMember("DecimalNum"))
					content.DecimalNum = jsonObj["DataContents"][i]["DecimalNum"].GetInt();
				Data[i] = content;
			}
		}
		if (jsonObj.HasMember("StatusVarIdRef") && jsonObj["StatusVarIdRef"].IsObject())
			StatusVarRef.Parse(jsonObj["StatusVarIdRef"]);
		if (jsonObj.HasMember("ResultVarIdRef") && jsonObj["ResultVarIdRef"].IsObject())
			ResultVarRef.Parse(jsonObj["ResultVarIdRef"]);
		if (jsonObj.HasMember("ProgressVarIdRef") && jsonObj["ProgressVarIdRef"].IsObject())
			ProgressVarRef.Parse(jsonObj["ProgressVarIdRef"]);
	}
	void FunctionSetup::DownloadRecipe::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("RecipeName") && !jsonObj["RecipeName"].IsNull())
		{
			std::string name = jsonObj["RecipeName"].GetString();
			memset(RecipeName, '\0', 128);
			memcpy(RecipeName, name.c_str(), name.size());
		}

		if (jsonObj.HasMember("Count"))
			Size = jsonObj["Count"].GetInt();
		if (jsonObj.HasMember("RecipeVarIdRef") && jsonObj["RecipeVarIdRef"].IsObject())
			RegVar.Parse(jsonObj["RecipeVarIdRef"]);
		if (jsonObj.HasMember("TransferVarIdRef") && jsonObj["TransferVarIdRef"].IsObject())
			TransferVarIdRef.Parse(jsonObj["TransferVarIdRef"]);
	}
	void FunctionSetup::UploadRecipe::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("RecipeName") && !jsonObj["RecipeName"].IsNull())
		{
			std::string name = jsonObj["RecipeName"].GetString();
			memset(RecipeName, '\0', 128);
			memcpy(RecipeName, name.c_str(), name.size());
		}
		if (jsonObj.HasMember("Count"))
			Size = jsonObj["Count"].GetInt();
		if (jsonObj.HasMember("RecipeVarIdRef") && jsonObj["RecipeVarIdRef"].IsObject())
			RegVar.Parse(jsonObj["RecipeVarIdRef"]);
		if (jsonObj.HasMember("TransferVarIdRef") && jsonObj["TransferVarIdRef"].IsObject())
			TransferVarIdRef.Parse(jsonObj["TransferVarIdRef"]);
	}
	void FunctionSetup::CallbackFunc::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("FuncName") && !jsonObj["FuncName"].IsNull())
		{
			//FunctionName = jsonObj["FunctionName"].GetString();
			std::string name = jsonObj["FuncName"].GetString();
			strcpy(CbFuncName, name.c_str());
			//memcpy(CbFuncName, name.c_str(), name.size());
		}
	}
	void FunctionSetup::PrintScreen::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("ScreenSrc"))
			ScreenSrc = (WinSrc)jsonObj["ScreenSrc"].GetInt();
		if (jsonObj.HasMember("ScreenVarIdRef") && jsonObj["ScreenVarIdRef"].IsObject())
			ScreenVarIdRef.Parse(jsonObj["ScreenVarIdRef"]);
		if (jsonObj.HasMember("WinNo"))
			WinNo = jsonObj["WinNo"].GetInt();
		if (jsonObj.HasMember("StorePos"))
			StorePos = jsonObj["StorePos"].GetInt();
		if (jsonObj.HasMember("PrintDir"))
			PrintDir = jsonObj["PrintDir"].GetInt();
		if (jsonObj.HasMember("PrintMode"))
			PrintMode = jsonObj["PrintMode"].GetInt();
		
	}
	void FunctionSetup::EditRecipe::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("RecipeEditAction"))
			Action = (RecipeEditAction)jsonObj["RecipeEditAction"].GetInt();
	}
	void BtnFunctionRes::Parse(std::vector<BtnFunctionRes>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<BtnFunctionRes>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			Project::BtnFunctionRes tmpFunc;
			if (jsonObj[i].HasMember("Name") && !jsonObj[i]["Name"].IsNull())
			{
				tmpFunc.FunctionName = jsonObj[i]["Name"].GetString();
				if (tmpFunc.FunctionName == "SetCoil")
					tmpFunc.FunctionParam.SetCoil.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "SetData")
					tmpFunc.FunctionParam.SetData.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "Arithmetic")
					tmpFunc.FunctionParam.Arithmetic.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "SendData")
					tmpFunc.FunctionParam.SendData.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "SwitchScreen")
					tmpFunc.FunctionParam.SwitchScreen.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "OpenWin")
					tmpFunc.FunctionParam.OpenWin.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "CloseWin")
					tmpFunc.FunctionParam.CloseWin.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "ImportCSVdata")
					tmpFunc.FunctionParam.ImportCSVdata.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "ExportCSVdata")
					tmpFunc.FunctionParam.ExportCSVdata.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "DownloadRecipe")
					tmpFunc.FunctionParam.DownloadRecipe.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "UploadRecipe")
					tmpFunc.FunctionParam.UploadRecipe.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "CallbackFunc")
					tmpFunc.FunctionParam.CallbackFunc.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "PrintScreen")
					tmpFunc.FunctionParam.PrintScreen.Parse(jsonObj[i]);
				if (tmpFunc.FunctionName == "EditRecipe")
					tmpFunc.FunctionParam.EditRecipe.Parse(jsonObj[i]);
			}
			if (jsonObj[i].HasMember("PromptNoPerm"))
				tmpFunc.PopTipWin = jsonObj[i]["PromptNoPerm"].GetBool();
			if (jsonObj[i].HasMember("PermRange"))
				tmpFunc.RequireRegion = jsonObj[i]["PermRange"].GetInt();
			vector.push_back(tmpFunc);
		}
	}
}
