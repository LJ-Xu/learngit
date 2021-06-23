#pragma once
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Table_Row.H>
#include "RecipeChartModel.h"
#include "RecipeChartControl.h"
#include "DataFormate.h"
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
namespace UI
{
	class RecipeChartView : public HMIBaseTable
	{
	public:
		RecipeChartView(int X, int Y, int W, int H, const char *l);
		~RecipeChartView();
		void InitDraw(void);
		void GetSearchData();
		string GetDrawString(string projectname, int row);
		bool HandleOperatePerm();

		BasicView BaseView;
		int DisplayMode = 0;					//0:����ģʽ 1:��ѯģʽ
		vector<int> SearchRows;
		//string SearchKey = "";					//��ѯ����
		//double SearchData;
		vector<vector<string>> RecipeDatas;		//�䷽������
	protected:
		void draw_cell(TableContext context,  		// table cell drawing
			int R = 0, int C = 0, int X = 0, int Y = 0, int W = 0, int H = 0);
		int handle(int event);
	private:
		int fontSize_;							//�б����ִ�С
		int fontStyle_;							//�б�����
		Fl_Color fontcolor_;
		Fl_Color cell_bgcolor_;					//�б�����ɫ
		Fl_Color focuscellcolor_;				//�б�ѡ�񱳾���ɫ
		Fl_Color focusfontcolor_;				//�б�ѡ��������ɫ
		Fl_Color focusrowcolor_;				//�б�ѡ������ɫ
		Fl_Color oddbgcolor_;
		Fl_Color evenbgcolor_;
		int datacol_ = 0;
		int clickcount_ = 0;								//��¼�������´���
		high_resolution_clock::time_point firstTime_;		//��¼��һ�ΰ�������ʱ��
		high_resolution_clock::time_point lastTime_;		//��¼���һ�ΰ�������ʱ��
		high_resolution_clock::time_point pushTime_;		//��¼��������ʱ��
		high_resolution_clock::time_point releaseTime_;		//��¼�����ɿ�ʱ��

		//Project::VarDataType selecteddatatype_;
		//Project::VarNumberType selecteddatafmt_;
		//Utility::NumberFmtInfo selectedfmt_;
		Project::RecipeInfoRes *selectresinfo_;
		int selectedcol_ = -1;					//ѡ�����
		int selectedrow_ = -1;					//ѡ�����
		int gridstarty_ = 0;

		bool haskeyboard_ = false;				//�Ƿ��м��̵���
		bool firstopen_ = true;			
		string inputtext_;
		bool hasPinYinpage = false;
		string pinyin_;
	};
}
