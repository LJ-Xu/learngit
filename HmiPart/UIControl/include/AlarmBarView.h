#pragma once
#include "stdafx.h"
#include "AlarmStorageService.h"
#include "BaseStorage.h"
#include "AlarmRecord.h"
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
	class AlarmBarControl;
	class AlarmBarModel;
	class AlarmBarView : public HMIBaseView
	{
	public:
		AlarmBarView(int X, int Y, int W, int H);
		~AlarmBarView();
		BasicView BaseView;
	protected:
		void draw() override;
		void DrawStickChart(AlarmBarControl* ctrl, AlarmBarModel* model);
	private:
		//void DrawScroll(void* v, int X, int Y, int W, int H);
		void SplitText(AlarmBarControl* ctrl, AlarmBarModel* model);
		void SplitTimeStr(tm time, int datetype, int clocktype, char* str,int buffLen);
		int CalcuCurStrBarLen(char* c);
	private:
#define SHOWNO		0x01
#define SHOWDATE	0x02
#define SHOWTIME	0x04
#define SHOWMSG		0x08
#define SHOWCOUNT	0x10
		bool Inited;
		int MainStrLen;
		int FinX;
		int FinY;
		//int ShowMode;
		char TitleTip[5];//1序号2触发日期3触发时间4报警信息5报警次数
		int CurAlarmIndex;
		std::string DrawString;
		//char StrAlarm[ALARMTEXTLEN];
		std::vector<std::string> AlarmStrList;
		std::vector<Storage::AlarmRecord> AlarmRecordList;
	public:
		int CurDx;//当前滚动偏移（以图形左上角为起点向左正数）
	};

}
