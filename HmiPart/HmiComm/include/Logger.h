#pragma once
#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/PropertyConfigurator.hh"
//#include "log4cpp/SyslogAppender.hh"
#include "log4cpp/LoggingEvent.hh"
#include <log4cpp/PatternLayout.hh>


class Logger
{
public:
	static Logger& Ins();
	static void SetFlag(bool);
	void Init(std::string path);
	log4cpp::Category& GetCategory(int mode = 0);
private:
	static bool SettableFlag;
	Logger();
	Logger(Logger&) = default;
	log4cpp::Category* root_;
	log4cpp::Category* cmd_;
};
//#define LOG_MODE_CONSOLE 1
//#define LOG_MODE_FILE    0
//#define LOG_MODE LOG_MODE_CONSOLE
//__FILE__  __LINE__
#define LOG_INFO(str,...) Logger::Ins().GetCategory().info(str,##__VA_ARGS__)
#define LOG_WARN(str,...) Logger::Ins().GetCategory().warn(str,##__VA_ARGS__)
#define LOG_ERROR(str,...) Logger::Ins().GetCategory().error(str,##__VA_ARGS__)
#define LOG_CRIT(str,...) Logger::Ins().GetCategory().crit(str,##__VA_ARGS__)
#define LOG_ALERT(str,...) Logger::Ins().GetCategory().alert(str,##__VA_ARGS__)
#define LOG_FLUSH() Logger::Ins().GetCategory().shutdown()

#define LOG_INFO_(msg) \
    Logger::Ins().GetCategory() <<log4cpp::Priority::INFO << \
     __FILE__ << " [" << __LINE__  << "] " << msg
    
#define LOG_WARN_(msg) \
    Logger::Ins().GetCategory() <<log4cpp::Priority::WARN << \
     __FILE__ << " [" << __LINE__  << "] " << msg

#define LOG_ERROR_(msg) \
    Logger::Ins().GetCategory() <<log4cpp::Priority::ERROR << \
     __FILE__ << " [" << __LINE__  << "] " << msg

#define LOG_CRIT_(msg) \
    Logger::Ins().GetCategory() <<log4cpp::Priority::CRIT << \
     __FILE__ << " [" << __LINE__  << "] " << msg

#define LOG_ALERT_(msg) \
    Logger::Ins().GetCategory() <<log4cpp::Priority::ALERT << \
     __FILE__ << " [" << __LINE__  << "] " << msg
