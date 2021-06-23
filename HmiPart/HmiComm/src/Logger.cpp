
#include "Logger.h"
#include "System.h"
bool Logger::SettableFlag = false;
Logger& Logger::Ins()
{
	static Logger log;
	return log;
}
Logger::Logger()
{
	if (!Logger::SettableFlag)
		Logger::Init("log.cfg");
}
void Logger::SetFlag(bool flag)
{
	Logger::SettableFlag = flag;
}
void Logger::Init(std::string path)
{
	
	if (cmd_)return;
	if (!path.empty())
	{
		try
		{
			log4cpp::PropertyConfigurator::configure(path);
		}
		catch (log4cpp::ConfigureFailure & f)
		{
			std::cerr << "configure problem " << f.what() << std::endl;
		}
		cmd_ = &log4cpp::Category::getInstance(std::string("hmilog"));
		return;
	}

	/*char buf[255];*/
	// 控制台标准
	log4cpp::OstreamAppender* cmdAppender = new log4cpp::OstreamAppender("default", &std::cout);
	cmdAppender->setLayout(new log4cpp::BasicLayout());
	//// 输出到文件program.log
	//log4cpp::Appender *  fileAppender = new log4cpp::FileAppender("default", System::GetFilePath("\\hmi.log", buf));
	//fileAppender->setLayout(new log4cpp::BasicLayout());

	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.setPriority(log4cpp::Priority::ERROR);
	root.addAppender(cmdAppender);
	root_ = &root;

	/*log4cpp::Category& cmd = root.getInstance("cmd");
	cmd.addAppender(cmdAppender);
	cmd.setPriority(log4cpp::Priority::INFO);*/
	cmd_ = &root;
}


log4cpp::Category& Logger::GetCategory(int mode)
{
	return *cmd_;
}