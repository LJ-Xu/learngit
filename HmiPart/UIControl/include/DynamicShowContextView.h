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
	class DynamicTextBarControl;
	class DynamicTextBarModel;
	class DynamicTextBarView : public HMIBaseView
	{
	public:
		DynamicTextBarView(int X, int Y, int W, int H);
		~DynamicTextBarView();
		BasicView BaseView;
	protected:
		void draw() override;
		void DrawStickChart(DynamicTextBarControl* ctrl, DynamicTextBarModel* model);
	private:
	private:
		bool Inited;
		int MainStrLen;
		int FinX;
		int FinY;
		int CurAlarmIndex;
		std::string DrawString;
	public:
		int CurDx;//当前滚动偏移（以图形左上角为起点向左正数）
		std::vector<std::string> ContextStrList;
	};

}
