/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeGControl.cpp
 * Author   : qiaodan
 * Date     : 2021-02-03
 * Descript : 创建配方数据库，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "RecipeGControl.h"
namespace UI
{
	RecipeGControl * RecipeGControl::instance_ = nullptr;

	RecipeGControl::RecipeGControl(HMIPage * pg) : BaseGControl(pg)
	{
		model_ = shared_ptr<RecipeGModel>(new RecipeGModel());
		InitMVCModel(model_);
		instance_ = this;
	}


	RecipeGControl * RecipeGControl::Ins() {
		static std::once_flag flag;
		if (!instance_)
		{
			call_once(flag, [&]() {
				instance_ = new RecipeGControl();
			});
		}
		return instance_;
	}
	
	void RecipeGControl::CreateView() {

	}

	int RecipeGControl::PeekHMIMessage(Message::Msg* msg)
	{
		return 1;
	}

	void RecipeGControl::HandleDataNotify(Project::DataVarId vid)
	{

	}
	bool RecipeGControl::InsertRecipeRecord(string groupname, int no)
	{
		map<string, vector<Project::RecipeInfoRes>>::iterator  iter;
		iter = model_->RecipeGConfig.TbList.find(groupname);
		if (iter != model_->RecipeGConfig.TbList.end())			//存在数据
		{
			if (iter->second.size() == no)
				return false;
			else
			{
				for (size_t i = 0; i < iter->second.size(); i++)
				{
					if (iter->second[i].RecipeData.size() >= no)
					{
						if (iter->second[i].RecipeName == "名称")
							iter->second[i].RecipeData.insert(iter->second[i].RecipeData.begin() + no," ");
						else
						{
							DDWORD data = 0;
							XJDataType tmptp;
							tmptp.Cls = iter->second[i].DataType;
							tmptp.Type = iter->second[i].DataFmt;
							Utility::NumberFmtInfo fmt;
							fmt.IsFillZero = false;
							fmt.Num1 = iter->second[i].IntegerNum;
							fmt.Num2 = iter->second[i].DecimalNum;
							string datastr = Utility::DataFormate::NumFmt(data, tmptp, fmt);
							iter->second[i].RecipeData.insert(iter->second[i].RecipeData.begin() + no, datastr);
						}
					}
						
				}
			}
		}
		return false;
	}
	bool RecipeGControl::DeleteRecipeRecord(string groupname, int no)
	{
		map<string, vector<Project::RecipeInfoRes>>::iterator  iter;
		iter = model_->RecipeGConfig.TbList.find(groupname);
		if (iter != model_->RecipeGConfig.TbList.end())			//存在数据
		{
			if (iter->second.size() == 0)
				return false;
			else
			{
				for (size_t i = 0; i < iter->second.size(); i++)
				{
					if (iter->second[i].RecipeData.size() > no)
					{
						iter->second[i].RecipeData.erase((iter->second[i].RecipeData.begin() + no));
					}
				}
				return true;
			}
		}
		return false;
	}
	bool RecipeGControl::AddRecipeRecord(string groupname)
	{
		map<string, vector<Project::RecipeInfoRes>>::iterator  iter;
		iter = model_->RecipeGConfig.TbList.find(groupname);
		if (iter != model_->RecipeGConfig.TbList.end())			//存在数据
		{
			if (iter->second.size() == 0)
				return false;
			else
			{
				for (size_t i = 0; i < iter->second.size(); i++)
				{
					if (iter->second[i].RecipeName == "名称")
						iter->second[i].RecipeData.push_back(" ");
					else
					{
						DDWORD data = 0;
						XJDataType tmptp;
						tmptp.Cls = iter->second[i].DataType;
						tmptp.Type = iter->second[i].DataFmt;
						Utility::NumberFmtInfo fmt;
						fmt.IsFillZero = false;
						fmt.Num1 = iter->second[i].IntegerNum;
						fmt.Num2 = iter->second[i].DecimalNum;
						string datastr = Utility::DataFormate::NumFmt(data, tmptp, fmt);
						iter->second[i].RecipeData.push_back(datastr);
					}
				}
				return true;
			}
		}
		return false;
	}
	string RecipeGControl::GetRecipeData(string groupname, string recipename, int rowno)
	{
		string data = "";
		map<string, vector<Project::RecipeInfoRes>>::iterator  iter;
		iter = model_->RecipeGConfig.TbList.find(groupname);
		if (iter != model_->RecipeGConfig.TbList.end())			//存在数据
		{
			for (size_t i = 0; i < iter->second.size(); i++)
			{
				if (iter->second[i].RecipeName == recipename)
				{
					if (iter->second[i].RecipeData.size() > rowno)
					{
						data = iter->second[i].RecipeData[rowno];
						return data;
					}
				}
			}
		}
		return data;
	}

	bool RecipeGControl::SetRecipeData(string groupname, string recipename, int rowno, string data)
	{
		map<string, vector<Project::RecipeInfoRes>>::iterator  iter;
		iter = model_->RecipeGConfig.TbList.find(groupname);
		if (iter != model_->RecipeGConfig.TbList.end())			//存在数据
		{
			for (size_t i = 0; i < iter->second.size(); i++)
			{
				if (iter->second[i].RecipeName == recipename)
				{
					if (iter->second[i].RecipeData.size() > rowno)
					{
						if(iter->second[i].RecipeName == "名称")
							iter->second[i].RecipeData[rowno] = data;
						else
						{
							XJDataType tmptp;
							tmptp.Cls = iter->second[i].DataType;
							tmptp.Type = iter->second[i].DataFmt;
							Utility::NumberFmtInfo fmt;
							fmt.IsFillZero = false;
							fmt.Num1 = iter->second[i].IntegerNum;
							fmt.Num2 = iter->second[i].DecimalNum;
							string datastr = Utility::DataFormate::StrFmt(data, tmptp, fmt);
							iter->second[i].RecipeData[rowno] = datastr;
						}
						return true;
					}
				}
			}
		}
		return false;
	}
	bool RecipeGControl::SearchRecipeData(string groupname, vector<int>& rownos, string data, int type)
	{
		map<string, vector<Project::RecipeInfoRes>>::iterator  iter;
		iter = model_->RecipeGConfig.TbList.find(groupname);
		if (iter != model_->RecipeGConfig.TbList.end())			//存在数据
		{
			for (size_t i = 0; i < iter->second.size(); i++)
			{
				if (type)
				{
					if (iter->second[i].RecipeName != "名称")
					{
						for (size_t j = 0; j < iter->second[i].RecipeData.size(); j++)
						{
							if (iter->second[i].RecipeData[j] == data)
							{
								vector <int>::iterator tmpint = find(rownos.begin(),rownos.end(), j);
								if (tmpint == rownos.end())		//不存在
									rownos.push_back(j);
							}
						}
					}
				}
				else
				{
					for (size_t j = 0; j < iter->second[i].RecipeData.size(); j++)
					{
						if (iter->second[i].RecipeData[j] == data)
						{
							vector <int>::iterator tmpint = find(rownos.begin(), rownos.end(), j);
							if (tmpint == rownos.end())		//不存在
								rownos.push_back(j);
						}
					}
				}
			}
			return true;
		}
		return false;
	}

	bool RecipeGControl::GetDataType(string groupname, string recipename, Project::VarDataType &datatype, Project::VarNumberType &datafmt, Utility::NumberFmtInfo &fmt)
	{
		map<string, vector<Project::RecipeInfoRes>>::iterator  iter;
		iter = model_->RecipeGConfig.TbList.find(groupname);
		if (iter != model_->RecipeGConfig.TbList.end())			//存在数据
		{
			for (size_t i = 0; i < iter->second.size(); i++)
			{
				if (iter->second[i].RecipeName == recipename)
				{
					datatype = iter->second[i].DataType;
					datafmt = iter->second[i].DataFmt;
					fmt.Num1 = iter->second[i].IntegerNum;
					fmt.Num2 = iter->second[i].DecimalNum;
					return true;
				}
			}
		}
		return false;
	}

	int RecipeGControl::UpLoadRecipe(string groupname, int row, BYTE * data)
	{
		return 0;
	}

	int RecipeGControl::DownLoadRecipe(string groupname, int row, Project::DataVarId plcvar)
	{
		return 0;
	}

	int RecipeGControl::GetRecipeDataNum(string groupname)
	{
		map<string, vector<Project::RecipeInfoRes>>::iterator  iter;
		if (model_->RecipeGConfig.TbList.size() == 0)
			return 0;
		iter = model_->RecipeGConfig.TbList.find(groupname);
		if (iter != model_->RecipeGConfig.TbList.end())			//存在数据
		{
			if (!iter->second.empty())
				return iter->second[0].RecipeData.size();
		}
		return 0;
	}

}