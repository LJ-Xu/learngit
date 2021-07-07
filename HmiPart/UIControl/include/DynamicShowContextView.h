#pragma once
#include "stdafx.h"
#include "BasicView.h"
#include <ctime>
#include <chrono>
#define ALARMTEXTLEN 128
#define MAINALARMTEXTLEN 512
using namespace std;
using namespace std::chrono;
typedef std::chrono::milliseconds MilliSecs;
namespace UI
{
	class DynamicTextContextControl;
	class DynamicTextContextModel;
	class DynamicTextContextView : public HMIBaseView
	{
	public:
		DynamicTextContextView(int X, int Y, int W, int H);
		~DynamicTextContextView();
		BasicView BaseView;
	protected:
		void draw() override;
		void 
		void DrawStickChart(DynamicTextContextControl* ctrl, DynamicTextContextModel* model);
	private:
	private:
		bool Inited;
		int MainStrLen;
		int FinX;
		int FinY;
		int CurAlarmIndex;
		std::string DrawString;
	public:
		int IntervalCharSize;//������ַ��������趨�ļ�����غ������Сת������
		int CurDx;//��ǰ����ƫ�ƣ���ͼ�����Ͻ�Ϊ�������������
		std::vector<std::string> ContextStrList;
	};

}
