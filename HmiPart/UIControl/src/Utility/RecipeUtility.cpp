#include "RecipeUtility.h"
#include "DataApi.h"
#include "RecipeStorage.h"
#include "UIData.h"
namespace UI
{
	RecipeUtility & RecipeUtility::Ins()
	{
		static RecipeUtility ins;
		ins.recipes = &Project::HMIProject::Ins->Tables.Recipe.RecipeTbVct;
		return ins;
	}
	void RecipeUtility::GetDataTypes(string groupname, vector<Project::ColDataTypeInfo>& info)
	{
		vector<Project::ColDataTypeInfo>().swap(info);
		map<string, Project::RecipeTable>::iterator  iter;
		iter = recipes->find(groupname);
		if (iter !=  recipes->end())			//存在数据
		{
			for (size_t i = 0; i < iter->second.RecipeItem.size(); i++)
			{
				if (iter->second.RecipeItem[i].ColTitle != "名称")
				{
					Project::ColDataTypeInfo tmpinfo;
					tmpinfo.ColTitle = iter->second.RecipeItem[i].ColTitle;
					tmpinfo.DataFmt = iter->second.RecipeItem[i].DataFmt;
					tmpinfo.DataType = iter->second.RecipeItem[i].DataType;
					tmpinfo.RegCount = iter->second.RecipeItem[i].RegCount;
					info.push_back(tmpinfo);
				}

			}
		}
	}

	Project::DataVarId RecipeUtility::GetIndexVar(string groupname)
	{
		map<string, Project::RecipeTable>::iterator  iter;
		iter = recipes->find(groupname);
		if (iter !=  recipes->end())			//存在数据
			return iter->second.IndexVar;
		else
			return Project::DataVarId::NullId;
	}

	Project::RecipeInfoRes* RecipeUtility::GetDataType(string groupname, string recipename)
	{
		map<string, Project::RecipeTable>::iterator  iter;
		iter = recipes->find(groupname);
		if (iter !=  recipes->end())			//存在数据
		{
			for (size_t i = 0; i < iter->second.RecipeItem.size(); i++)
			{
				if (iter->second.RecipeItem[i].ColTitle == recipename)
				{
					//RecipeInfoRes *res = &iter->second.RecipeItem[i];
					return  &iter->second.RecipeItem[i];
				}
			}
		}
		return NULL;
	}

	int RecipeUtility::GetColNum(string groupname, string recipename)
	{
		map<string, Project::RecipeTable>::iterator  iter;
		iter = recipes->find(groupname);
		if (iter !=  recipes->end())			//存在数据
		{
			for (size_t i = 0; i < iter->second.RecipeItem.size(); i++)
			{
				if (iter->second.RecipeItem[i].ColTitle == recipename)
					return  i;
			}
		}
		return -1;
	}
	int RecipeUtility::GetColNum(string groupname)
	{
		map<string, Project::RecipeTable>::iterator  iter;
		iter = recipes->find(groupname);
		if (iter !=  recipes->end())			//存在数据
			return iter->second.RecipeItem.size();
		return 0;
	}
	void RecipeUtility::RecordFocus(string groupname, int row)
	{
		focusgroupname_ = groupname;
		focusrow_ = row;
	}
	void RecipeUtility::Add()
	{
		if (Storage::RecipeStorage::Ins()->AddRecord(focusgroupname_))		//添加成功
		{
			bool state = LocalData::GetBit(SYS_PSB_ReciprRECORD_NOTICE);
			LocalData::SetBit(SYS_PSB_ReciprRECORD_NOTICE,!state);			//通知更新
		}
	}
	void RecipeUtility::Copy()
	{
		int cols = GetColNum(focusgroupname_);
		vector<Project::ColDataTypeInfo> ColInfo;
		RecipeDT::Ins()->GetDataTypes(focusgroupname_, ColInfo);
		if (Storage::RecipeStorage::Ins()->CopyRecord(focusgroupname_, focusrow_, ColInfo))		//复制成功
		{
			bool state = LocalData::GetBit(SYS_PSB_ReciprRECORD_NOTICE);
			LocalData::SetBit(SYS_PSB_ReciprRECORD_NOTICE, !state);			//通知更新
		}
	}
	void RecipeUtility::Insert()
	{
		if (Storage::RecipeStorage::Ins()->InsertRecord(focusgroupname_, focusrow_))		//插入成功
		{
			bool state = LocalData::GetBit(SYS_PSB_ReciprRECORD_NOTICE);
			LocalData::SetBit(SYS_PSB_ReciprRECORD_NOTICE, !state);			//通知更新
		}
	}
	void RecipeUtility::Delete()
	{
		if (Storage::RecipeStorage::Ins()->DeleteRecord(focusgroupname_,focusrow_))		//删除成功
		{
			bool state = LocalData::GetBit(SYS_PSB_ReciprRECORD_NOTICE);
			LocalData::SetBit(SYS_PSB_ReciprRECORD_NOTICE, !state);			//通知更新
		}
	}
	void RecipeUtility::MoveUp()
	{
		if (Storage::RecipeStorage::Ins()->MoveUpRecord(focusgroupname_, focusrow_))		//向上移成功
		{
			bool state = LocalData::GetBit(SYS_PSB_ReciprRECORD_NOTICE);
			LocalData::SetBit(SYS_PSB_ReciprRECORD_NOTICE, !state);			//通知更新
		}
	}
	void RecipeUtility::MoveDown()
	{
		if (Storage::RecipeStorage::Ins()->MoveDownRecord(focusgroupname_, focusrow_))		//向下移成功
		{
			bool state = LocalData::GetBit(SYS_PSB_ReciprRECORD_NOTICE);
			LocalData::SetBit(SYS_PSB_ReciprRECORD_NOTICE, !state);			//通知更新
		}
	}
	void RecipeUtility::UpdateData(string groupname, string colname, unsigned int rowno, string data)
	{
		if (Storage::RecipeStorage::Ins()->UpdateRecord(groupname, colname, rowno, data))	//设置数据成功
		{
			bool state = LocalData::GetBit(SYS_PSB_ReciprRECORD_NOTICE);
			LocalData::SetBit(SYS_PSB_ReciprRECORD_NOTICE, !state);			//通知更新
		}
	}
}
