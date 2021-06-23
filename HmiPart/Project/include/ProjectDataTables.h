#pragma once
#include "RecipeInfoRes.h"
#include <string>
#include <vector>
#include "CerealCommon.hpp"

using namespace std;
namespace Project
{
#define MAX_LANG_COUNT 10
	/******************�û���Ϣ*********************/
	struct UserItem
	{
		string Uname; //�û���
		string Passwd;//����
		int Region; //Ȩ����
		bool Hide;//�Ƿ�����
		bool Enable;//�Ƿ�ʹ��

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Uname, Passwd, Region, Hide, Enable);
		}
	};
	struct UserDT
	{
		vector<UserItem> Users;//�û��˺�
		UserItem Admin;  //����Ա�˺�
		vector<string> ColDescript;	//Ȩ����������Ϣ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Users, Admin);
		}
	};

	/******************�û���Ϣ*��ǩ״̬�� ���״̬********************/
	struct StringStatusItem
	{
		int Id;   // ���
		string Tag;  // ��ǩ
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

	/******************�ַ�����Ϣ*�ַ�����,ͨ��ID����********************/
	struct StringLabelTable
	{
		// ���֎��
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

	/******************�䷽��Ϣ*********************/
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
		 * Descript : ɾ���䷽����
		 * Input    : groupname���䷽����
		 *			  no:ɾ�����
		 * Output   : false��ɾ��ʧ��
		 *			��true��ɾ���ɹ�
		 * Note	    :
		 *******************************************************************************/
		bool DeleteRecipeRecord(string groupname, unsigned int no);
		/*******************************************************************************
		 * Name     : InsertRecipeRecord
		 * Descript : �����䷽����
		 * Input    : groupname���䷽����
		 *			  no:�������
		 * Output   : false������ʧ��
		 *			��true������ɹ�
		 * Note	    :
		 *******************************************************************************/
		bool InsertRecipeRecord(string groupname, unsigned int no, string datastr = nullptr);
		/*******************************************************************************
		 * Name     : AddRecipeRecord
		 * Descript : ����䷽����
		 * Input    : groupname���䷽����
		 * Output   : false�����ʧ��
		 *			��true����ӳɹ�
		 * Note	    :
		 *******************************************************************************/
		bool AddRecipeRecord(string groupname, string datastr = nullptr);
		/*******************************************************************************
		 * Name     : GetRecipeData
		 * Descript : ��ȡ�䷽����
		 * Input    : groupname���䷽����
		 *			  recipename���䷽����
		 *			  rowno���к�
		 * Output   : string������
		 * Note	    :
		 *******************************************************************************/

		bool SetRecipeData(string groupname, string recipename, unsigned int rowno, string data);
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

		int UpLoadRecipe(string groupname, int row, char* data);		//�ϴ��䷽
		int DownLoadRecipe(string groupname, int row, Project::DataVarId plcvar);
#endif
	private:
		static RecipeDT* ins_;
	};

	/******************�¼���Ϣ*********************/
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

	/******************������Ϣ*********************/
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

	
	/******************���ݱ��������*********************/
	struct ProjectDataTables
	{
		// �û��б�
		UserDT User;

		// ��ǩ״̬�� ���״̬
		StringStatusDT StrStatus;

		// �ַ�����,ͨ��ID����
		StringLabelDT StrLabel;

		// �䷽��
		RecipeDT Recipe;

		// �¼���
		EventDT Event;

		// ������Ϣ��
		SampleDT Simple;


		template<class Archive>
		void serialize(Archive & archive)
		{			
			archive(User, StrStatus, StrLabel, Recipe, Event, Simple);
		}
		void InitData(std::string jstr);
	};

}