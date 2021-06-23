#pragma once
#include "stdafx.h"
#include "BasicView.h"
#include "AlarmInfoRes.h"
namespace UI
{
	class AlarmGControl;
	class AlarmGModel;

	class AlarmGView : public HMIBaseView
	{
	public:
		AlarmGView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H) {
			/*Triggeralarm = false;
			UseBeep = false;
			UseAlarmPopWin = false;
			IsCloseWin = false;
			BeepTimeOut = 0;
			PopWinNo = 25001;*/
		}
		~AlarmGView() {}
		BasicView BaseView;
	protected:
		void draw() override;
	public:
		/*bool Triggeralarm;
		bool UseBeep;
		bool UseAlarmPopWin;
		bool IsCloseWin;
		std::atomic<int> BeepTimeOut;
		int PopWinNo;*/
		std::atomic<int> BeepTimeOut;
		std::atomic<bool> IsBeeping;
		std::queue<std::pair<bool, Project::AlarmInfoRes>> AlarmList;
	};
}