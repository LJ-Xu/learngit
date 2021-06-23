/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeGControl.h
 * Author   : qiaodan
 * Date     : 2021-02-03
 * Descript : �䷽�༭ͷ�ļ�
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
		 * Descript : ɾ���䷽����
		 * Input    : groupname���䷽����
		 *			  no:ɾ�����
		 * Output   : false��ɾ��ʧ��
		 *			��true��ɾ���ɹ�
		 * Note	    :
		 *******************************************************************************/
		bool DeleteRecipeRecord(string groupname, int no);
		/*******************************************************************************
		 * Name     : InsertRecipeRecord
		 * Descript : �����䷽����
		 * Input    : groupname���䷽����
		 *			  no:�������
		 * Output   : false������ʧ��
		 *			��true������ɹ�
		 * Note	    :
		 *******************************************************************************/
		bool InsertRecipeRecord(string groupname, int no);
		/*******************************************************************************
		 * Name     : AddRecipeRecord
		 * Descript : ����䷽����
		 * Input    : groupname���䷽����
		 * Output   : false�����ʧ��
		 *			��true����ӳɹ�
		 * Note	    :
		 *******************************************************************************/
		bool AddRecipeRecord(string groupname);
		/*******************************************************************************
		 * Name     : GetRecipeData
		 * Descript : ��ȡ�䷽����
		 * Input    : groupname���䷽����
		 *			  recipename���䷽����
		 *			  rowno���к�
		 * Output   : string������
		 * Note	    :
		 *******************************************************************************/
		string GetRecipeData(string groupname, string recipename, int rowno);
		bool SetRecipeData(string groupname, string recipename, int rowno, string data);

		/*******************************************************************************
		 * Name     : SearchRecipeData
		 * Descript : ��ѯ����Ϊdata���䷽
		 * Input    : groupname���䷽����
		 *			  rownos����ѯ�����к�
		 *            data����Ҫ��ѯ������
		 *			  type����ѯ���� 0:��ѯ�ؼ��� 1:��ѯ����
		 * Output   : false����ѯʧ��
		 *			��true����ѯ�ɹ�
		 * Note	    :
		 *******************************************************************************/
		bool SearchRecipeData(string groupname, vector<int>& rownos, string data, int type);
		int GetRecipeDataNum(string groupname);
		bool GetDataType(string groupname, string recipename, Project::VarDataType &datatype, Project::VarNumberType &datafmt, Utility::NumberFmtInfo &fmt);

		int UpLoadRecipe(string groupname, int row, BYTE* data);		//�ϴ��䷽
		int DownLoadRecipe(string groupname, int row, Project::DataVarId plcvar);
	private:
		shared_ptr<RecipeGModel> model_;
		static RecipeGControl * instance_;

	};
}

