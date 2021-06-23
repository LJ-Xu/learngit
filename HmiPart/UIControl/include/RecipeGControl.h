/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeGControl.h
 * Author   : qiaodan
 * Date     : 2021-02-03
 * Descript : 配方编辑头文件
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseGControl.h"
#include "RecipeGModel.h"
#include <BaseControl.h>
#include "DataFormate.h"
#include <vector>
using namespace std;

namespace UI
{
	class RecipeGControl : public BaseGControl
	{
	public:
		RecipeGControl() : RecipeGControl(nullptr) { }
		RecipeGControl(HMIPage *);
		~RecipeGControl() {}
		void CreateView() override;
		int PeekHMIMessage(Message::Msg * msg);
		void HandleDataNotify(Project::DataVarId id);
		static RecipeGControl * Ins();

		/*******************************************************************************
		 * Name     : DeleteRecipeRecord
		 * Descript : 删除配方数据
		 * Input    : groupname：配方表名
		 *			  no:删除序号
		 * Output   : false：删除失败
		 *			：true：删除成功
		 * Note	    :
		 *******************************************************************************/
		bool DeleteRecipeRecord(string groupname, int no);
		/*******************************************************************************
		 * Name     : InsertRecipeRecord
		 * Descript : 插入配方数据
		 * Input    : groupname：配方表名
		 *			  no:插入序号
		 * Output   : false：插入失败
		 *			：true：插入成功
		 * Note	    :
		 *******************************************************************************/
		bool InsertRecipeRecord(string groupname, int no);
		/*******************************************************************************
		 * Name     : AddRecipeRecord
		 * Descript : 添加配方数据
		 * Input    : groupname：配方表名
		 * Output   : false：添加失败
		 *			：true：添加成功
		 * Note	    :
		 *******************************************************************************/
		bool AddRecipeRecord(string groupname);
		/*******************************************************************************
		 * Name     : GetRecipeData
		 * Descript : 获取配方数据
		 * Input    : groupname：配方表名
		 *			  recipename：配方列名
		 *			  rowno：行号
		 * Output   : string：数据
		 * Note	    :
		 *******************************************************************************/
		string GetRecipeData(string groupname, string recipename, int rowno);
		bool SetRecipeData(string groupname, string recipename, int rowno, string data);

		/*******************************************************************************
		 * Name     : SearchRecipeData
		 * Descript : 查询数据为data的配方
		 * Input    : groupname：配方表名
		 *			  rownos：查询到的行号
		 *            data：需要查询的数据
		 *			  type：查询类型 0:查询关键字 1:查询数据
		 * Output   : false：查询失败
		 *			：true：查询成功
		 * Note	    :
		 *******************************************************************************/
		bool SearchRecipeData(string groupname, vector<int>& rownos, string data, int type);
		int GetRecipeDataNum(string groupname);
		bool GetDataType(string groupname, string recipename, Project::VarDataType &datatype, Project::VarNumberType &datafmt, Utility::NumberFmtInfo &fmt);

		int UpLoadRecipe(string groupname, int row, BYTE* data);		//上传配方
		int DownLoadRecipe(string groupname, int row, Project::DataVarId plcvar);
	private:
		shared_ptr<RecipeGModel> model_;
		static RecipeGControl * instance_;

	};
}

