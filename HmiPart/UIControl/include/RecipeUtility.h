#pragma once
#include "HMIProject.h"
#include "RecipeChartControl.h"
namespace UI
{
	class RecipeUtility
	{
	public:
		static RecipeUtility& Ins();

		Project::RecipeInfoRes* GetDataType(string groupname, string projectname);
		void GetDataTypes(string groupname, vector<Project::ColDataTypeInfo>& info);
		Project::DataVarId GetIndexVar(string groupname);
		int GetColNum(string groupname, string projectname);
		int GetColNum(string groupname);

		void RecordFocus(string groupname, int row);

		void Add();			//增加
		void Copy();		//复制
		void Insert();		//插入
		void Delete();		//删除
		void MoveUp();
		void MoveDown();
		void UpdateData(string groupname, string colname, unsigned int rowno, string data);
	private:
		map<string, Project::RecipeTable>* recipes = nullptr;
		string focusgroupname_;
		int focusrow_ = -1;
		RecipeChartControl* focusctrl = nullptr;
	};
}

