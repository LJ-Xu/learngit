#pragma once
#include "RecipeInfoRes.h"
#include <string>
#include <vector>
#include "CerealCommon.hpp"

using namespace std;
namespace Project
{
#define MAX_LANG_COUNT 10
	/******************用户信息*********************/
	struct UserItem
	{
		string Uname; //用户名
		string Passwd;//密码
		int Region; //权限域
		bool Hide;//是否隐藏
		bool Enable;//是否使用

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Uname, Passwd, Region, Hide, Enable);
		}
	};
	struct UserDT
	{
		vector<UserItem> Users;//用户账号
		UserItem Admin;  //管理员账号
		vector<string> ColDescript;	//权限域描述信息

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Users, Admin);
		}
	};

	/******************用户信息*标签状态库 多个状态********************/
	struct StringStatusItem
	{
		int Id;   // 序号
		string Tag;  // 标签
		vector<vector<string>> StateLangs;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Id, Tag, StateLangs);
		}
	};
	struct StringStatusDT
	{
		//vector<StringStatusItem> stringStatusDT;
		std::map<string, StringStatusItem> StringStatusMap;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(StringStatusMap);
		}
		const char* GetString(const char* label, int state, int lang)
		{
			if (StringStatusMap.find(label) == StringStatusMap.end())
				return "";
			if (StringStatusMap[label].StateLangs.size() <= (size_t)state || StringStatusMap[label].StateLangs[state].size() <= (size_t)lang)
				return "";
			return StringStatusMap[label].StateLangs[state][lang].c_str();
		}
		
	};

	/******************字符串信息*字符串库,通过ID索引********************/
	struct StringLabelTable
	{
		// 文字庫表
		vector<vector<string>> Rows;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Rows);
		}
		
	};
	struct StringLabelDT
	{
		vector<StringLabelTable> StrLabelVct;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(StrLabelVct);
		}

		const char* GetString(int tableId, int idx, int state, int lang)
		{
			if (tableId >= (int)StrLabelVct.size())
				return nullptr;
			return StrLabelVct[tableId].Rows[state][lang].c_str();
		}
	};

	/******************配方信息*********************/
	struct RecipeTable
	{
		vector<RecipeInfoRes> RecipeItem;
		DataVarId  IndexVar;
		bool OutAddrUsed;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(RecipeItem, IndexVar, OutAddrUsed);
		}

	};
	struct RecipeDT
	{
		map<string, RecipeTable> RecipeTbVct;
		//vector<RecipeItem> RecipeVct;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(RecipeTbVct);
		}
		static RecipeDT* Ins();
		static void SetRecipeDTIns(RecipeDT*);
		RecipeInfoRes* GetDataType(string groupname, string projectname);
		void GetDataTypes(string groupname, vector<ColDataTypeInfo>& info);
		DataVarId GetIndexVar(string groupname);
		int GetColNum(string groupname, string projectname);
		int GetColNum(string groupname);
#if 0
		string GetRecipeData(string groupname, string recipename, unsigned int rowno);
		int GetRecipeDataNum(string groupname);

		/*******************************************************************************
		 * Name     : DeleteRecipeRecord
		 * Descript : 删除配方数据
		 * Input    : groupname：配方表名
		 *			  no:删除序号
		 * Output   : false：删除失败
		 *			：true：删除成功
		 * Note	    :
		 *******************************************************************************/
		bool DeleteRecipeRecord(string groupname, unsigned int no);
		/*******************************************************************************
		 * Name     : InsertRecipeRecord
		 * Descript : 插入配方数据
		 * Input    : groupname：配方表名
		 *			  no:插入序号
		 * Output   : false：插入失败
		 *			：true：插入成功
		 * Note	    :
		 *******************************************************************************/
		bool InsertRecipeRecord(string groupname, unsigned int no, string datastr = nullptr);
		/*******************************************************************************
		 * Name     : AddRecipeRecord
		 * Descript : 添加配方数据
		 * Input    : groupname：配方表名
		 * Output   : false：添加失败
		 *			：true：添加成功
		 * Note	    :
		 *******************************************************************************/
		bool AddRecipeRecord(string groupname, string datastr = nullptr);
		/*******************************************************************************
		 * Name     : GetRecipeData
		 * Descript : 获取配方数据
		 * Input    : groupname：配方表名
		 *			  recipename：配方列名
		 *			  rowno：行号
		 * Output   : string：数据
		 * Note	    :
		 *******************************************************************************/

		bool SetRecipeData(string groupname, string recipename, unsigned int rowno, string data);
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

		int UpLoadRecipe(string groupname, int row, char* data);		//上传配方
		int DownLoadRecipe(string groupname, int row, Project::DataVarId plcvar);
#endif
	private:
		static RecipeDT* ins_;
	};

	/******************事件信息*********************/
	struct EventItem
	{
		int id;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(id);
		}
	};
	struct EventDT
	{
		vector<EventItem> eventTB;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(eventTB);
		}
	};

	/******************采样信息*********************/
	struct SampleItem
	{
		int id;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(id);
		}
	};
	struct SampleDT
	{
		vector<SampleItem> SampleVct;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(SampleVct);
		}
	};

	
	/******************数据表相关内容*********************/
	struct ProjectDataTables
	{
		// 用户列表
		UserDT User;

		// 标签状态库 多个状态
		StringStatusDT StrStatus;

		// 字符串库,通过ID索引
		StringLabelDT StrLabel;

		// 配方表
		RecipeDT Recipe;

		// 事件表
		EventDT Event;

		// 采样信息表
		SampleDT Simple;


		template<class Archive>
		void serialize(Archive & archive)
		{			
			archive(User, StrStatus, StrLabel, Recipe, Event, Simple);
		}
		void InitData(std::string jstr);
	};

}