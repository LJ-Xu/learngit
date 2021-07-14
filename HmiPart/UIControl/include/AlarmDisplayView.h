#pragma once
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Table_Row.H>
#include "AlarmInfoRes.h"
#include "AlarmDisplayModel.h"
#include <time.h>
#include "AlarmRecord.h"
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace UI
{

	class AlarmDisplayView : public HMIBaseTable
	{
	public:
		AlarmDisplayView(int X, int Y, int W, int H, const char *l);
		~AlarmDisplayView();
		BasicView BaseView;
		Project::AlarmDisMode DisplayMode = Project::AlarmDisMode::REALTIME;		//��ʾģʽ 0��ʵʱ	1����ʷ 2����ѯ
		int HideFlag;									//���ؿ���
		vector<Storage::AlarmRecord> DisplayInfo;		//������ʾ����Ϣ

		void InitDraw(void);							//��ʼ��������ʾ���ƵĲ���
		int handle(int event);							//������ȷ��
		int GetInfoNum();								//��ȡ��ʾ��Ϣ����ʾ��Ϣ����
		string GetTimeString(DDWORD time, int mode);	//��ʱ��ת���ַ���
		string GetDrawString(string projectname, int row);			//��ȡҪ���Ƶ��ַ���
		void SortAlarmInfo();							//��ȡ��������ʾ��Ϣ���ö�δ�ָ���Ϣ��

		void SetRenderData();
	protected:
		void draw_cell(TableContext context,  		// table cell drawing
			int R = 0, int C = 0, int X = 0, int Y = 0, int W = 0, int H = 0);
	private:
		int fontSize_;					//�б����ִ�С
		int fontStyle_;					//�б�����
		Fl_Color fontColor_;			//�б�������ɫ
		int fontAlign_;					//�б����ֶ��뷽ʽ
		int gridstarty_ = 0;
		int gridstartx_ = 0;
		int gridstartcol_ = 0;

		Fl_Color cell_bgcolor_;			//�б�����ɫ
		Fl_Color triggerTextColor_;		//��������ʱ������ɫ
		Fl_Color triggerbgColor_;		//��������ʱ������ɫ
		Fl_Color confirmTextColor_;		//����ȷ��ʱ������ɫ
		Fl_Color confirmbgColor_;		//����ȷ��ʱ������ɫ
		Fl_Color recoverTextColor_;		//�����ָ�ʱ������ɫ
		Fl_Color recoverbgColor_;		//�����ָ�ʱ������ɫ

		int clickcount_ = 0;								//��¼�������´���
		high_resolution_clock::time_point firstTime_;		//��¼��һ�ΰ�������ʱ��
		high_resolution_clock::time_point lastTime_;		//��¼���һ�ΰ�������ʱ��
		high_resolution_clock::time_point pushTime_;		//��¼��������ʱ��
		high_resolution_clock::time_point releaseTime_;		//��¼�����ɿ�ʱ��

	};

}