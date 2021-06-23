#include "TrendChart_Test_Run.h"
#include <iostream>
#include <string>
#include <memory.H>
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>
#include "HMIWindow.h"
#include "TrendChartControl.h"
#include "TrendChartView.h"
int SampleCyc = 500;
TrendChart_Test_Run& TrendChart_Test_Run::Ins()
{
	TrendChart_Test_Run ins;
	return ins;
}
#if 0
void btncb(Fl_Widget*, void* data) {
	UI::TrendChartView* view = (UI::TrendChartView*)data;
	Project::SampleChannel::SampleContent samplecontent;
	samplecontent.ChannelNo = 1;
	timeval tv;
	gettimeofday(&tv, NULL);
	samplecontent.SampleTime.tv_sec = tv.tv_sec;
	samplecontent.SampleTime.tv_usec = tv.tv_usec;
	/*产生随机数*/
	srand((unsigned)tv.tv_sec);
	samplecontent.Data = std::to_string((rand() % (100 - 0 + 1)) + 0);
	view->SampleData[0].push_back(samplecontent);
	view->redraw();
}

void SampleTimerCb(void* data)
{
	UI::TrendChartView* view = (UI::TrendChartView*)data;
	Project::SampleChannel::SampleContent samplecontent;
	Project::SampleChannel::SampleContent samplecontent1;
	samplecontent.ChannelNo = 1;
	samplecontent1.ChannelNo = 2;
	timeval tv;
	gettimeofday(&tv, NULL);
	samplecontent.SampleTime.tv_sec = tv.tv_sec;
	samplecontent.SampleTime.tv_usec = tv.tv_usec;
	samplecontent1.SampleTime.tv_sec = tv.tv_sec;
	samplecontent1.SampleTime.tv_usec = tv.tv_usec;
	/*产生随机数*/
	srand((unsigned)tv.tv_sec);
	samplecontent.Data = std::to_string((rand() % (100 - 0 + 1)) + 0);
	samplecontent1.Data = std::to_string((rand() % (100 - 0 + 1)) + 0);

	view->SampleData[0].push_back(samplecontent);
	view->SampleData[1].push_back(samplecontent1);

	Fl::repeat_timeout((double)SampleCyc / 1000.0, SampleTimerCb, data);
	view->redraw();
}
#endif
bool TrendChart_Test_Run::Run(int argc, char ** argv)
{
#if 0
	UI::HMIWindow *window;
	window = new UI::HMIWindow(0,0,800, 800);
	Fl_Button * btn = new Fl_Button(40, 40, 80, 25, "&Beep1");
	btn->when(FL_WHEN_RELEASE);

	//b1->callback(opencb, 0);

	std::shared_ptr<UI::TrendChartControl> ctrl(new UI::TrendChartControl(window));
	std::shared_ptr<UI::BaseControl> pctrl = std::static_pointer_cast<UI::BaseControl>(ctrl);
	HMIBaseView* view = pctrl->CreateView("cd");
	std::shared_ptr<UI::TrendChartView> trendview((UI::TrendChartView*)view);
	vector<Project::SampleChannel::SampleContent> sampledata;
	trendview->SampleData.push_back(sampledata);
	vector<Project::SampleChannel::SampleContent> sampledata2;
	trendview->SampleData.push_back(sampledata2);
	btn->callback(btncb, (void *)trendview.get());

	window->end();
	window->show(argc, argv);
	
	Fl::add_timeout((double)SampleCyc / 1000.0, SampleTimerCb, (void *)trendview.get());
#endif
	//window->SendUserMessage(ctr, WM_EVENT_READY, 11);
	//Fl::add_timeout(0.01, (Fl_Timeout_Handler)[](void* data){xx(0);}, nullptr);
	return Fl::run();
}