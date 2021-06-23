#include "Log4cpp_Test_Use.h"
#include <iostream>
#include <string>
#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/RollingFileAppender.hh>
using namespace std;
Log4cpp_Test_Use& Log4cpp_Test_Use::Ins()
{
	Log4cpp_Test_Use ins;
	return ins;
}

bool Log4cpp_Test_Use::Run(int argc, char ** argv)
{
	log4cpp::PatternLayout* pLayout1 = new log4cpp::PatternLayout();//创建一个Layout;
	pLayout1->setConversionPattern("%d: %p %c %x: %m%n");//指定布局格式;

	log4cpp::PatternLayout* pLayout2 = new log4cpp::PatternLayout();
	pLayout2->setConversionPattern("%d: %p %c %x: %m%n");

	log4cpp::Appender* fileAppender = new log4cpp::FileAppender("fileAppender", "wxb.log");//创建一个Appender;
	fileAppender->setLayout(pLayout1);//将指定的Layout添加到Appender;

	log4cpp::RollingFileAppender* rollfileAppender = new log4cpp::RollingFileAppender(
		"rollfileAppender", "rollwxb.log", 5 * 1024, 1);
	rollfileAppender->setLayout(pLayout2);

	log4cpp::Category& root = log4cpp::Category::getRoot().getInstance("RootName");//从系统中得到Category的根;
	root.addAppender(fileAppender);//将Appender添加到Category;
	root.addAppender(rollfileAppender);
	root.setPriority(log4cpp::Priority::DEBUG);//设置Category的优先级;

	//开始记录日志;
	for (int i = 0; i < 100; i++)
	{
		string strError;
		ostringstream oss;
		oss << i << ":Root Error Message!";
		strError = oss.str();
		root.error(strError);
	}

	log4cpp::Category::shutdown();//关闭Category;
	printf("Log4cpp_Test_Use\n");
	return true;
}

