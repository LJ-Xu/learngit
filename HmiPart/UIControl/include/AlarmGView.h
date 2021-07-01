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
		}
		~AlarmGView() {}
		BasicView BaseView;
	protected:
		void draw() override {}
	public:
		/*std::vector<std::pair<bool, Project::AlarmInfoRes>> AlarmBeepList;
		std::vector<std::pair<bool, Project::AlarmInfoRes>> AlarmPopWinList;*/
	};
}