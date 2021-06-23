#pragma once
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Table_Row.H>
#include "DataTableModel.h"
#include "DataTableControl.h"
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
namespace UI
{
	enum Language {Charactinput = 0,Chineseinput = 1};
	class DataTableView : public HMIBaseTable
	{
	public:

		DataTableView(int X, int Y, int W, int H, const char *l);
		~DataTableView();
		void InitDraw(void);
		BasicView BaseView;
		int handle(int event);
		string GetDrawString(int row, int col);
		void EnterValue();
		bool HandleOperateConfirm();
		void EndInput();
		bool HandleOperatePerm();
		void InitStartInput(int mx, int my);
	protected:
		void draw_cell(TableContext context,  		// table cell drawing
			int R = 0, int C = 0, int X = 0, int Y = 0, int W = 0, int H = 0);
	private:
		int datacolnum_;						//��������
		int scrollsize_ = 0;
		int fontSize_;							//�б����ִ�С
		int fontStyle_;							//�б�����
		bool useserial_ = false;
		bool usestaticcol_ = false;
		Fl_Color cell_bgcolor_;					//�б�����ɫ
		int gridstarty_ = 0;
		int gridstartrow_ = 0;
		bool haskeyboard_ = false;				//�Ƿ��м��̵���
		bool hasPinYinpage = false;
		bool firstopen_ = true;

		Project::DataCell selectcell_;
		int selectedcol_ = -1;					//ѡ�����
		int selectedrow_ = -1;					//ѡ�����


		int clickcount_ = 0;								//��¼�������´���
		high_resolution_clock::time_point firstTime_;		//��¼��һ�ΰ�������ʱ��
		high_resolution_clock::time_point lastTime_;		//��¼���һ�ΰ�������ʱ��
		high_resolution_clock::time_point pushTime_;		//��¼��������ʱ��
		high_resolution_clock::time_point releaseTime_;		//��¼�����ɿ�ʱ��
	
		string inputtext_;
		string pinyin_;
		vector<int> languageflag_;				//true�����ģ�false���ַ�
	};
}