#pragma once
#include "BtnUnit.h"
#include "MoveableDisplayUnit.h"
#include <time.h>
//using namespace boost::asio;
namespace Project
{
	struct StaticHirePurchaseSet
	{
		time_t EndTime;
		std::string Description;
		//int StartTime;
		std::string Password;
		bool Parse(rapidjson::Value& jsonObj);
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(EndTime, Description, Password);
		}
	};
	class CHirePurchase : public MoveableDisplayUnit
	{
	public:
		//
		/*void Start();
		void Stop();*/
		void Parse(rapidjson::Value& jsonObj);
		//当前验证模式(0静态1动态)
		bool IsStatic() {
			return UsedStaticPurchase;
		}
		bool IsDynamic() {
			return UsedDynamicPurchase;
		}
		//日期字符串解析，固定解析"2021/5/27"+"19:36:53"的格式
		static time_t GetTime(const std::string&  strDate, const std::string&  strTime);
		bool VerifyAdminPassword(const std::string& pwd);
		bool VerifyCurDate(const std::string& pwd);
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(UsedStaticPurchase, UsedAdminPassword, UsedDynamicPurchase,   DynamicEndTime, Days, AdminPassword,
				Key, HmiId, StaticHirePurchaseSetQueue);
		}
	private:
		//开始新一轮倒计时（发生两个过期的时间则抛弃）
		void OnSpanner();
		//与当前时间比较
		//返回0时过去时(需要作验证)，1时未来时
		int CmpTm(time_t time);
	private:
		bool isRunning;
		bool Verifiyed;//是否已验证过
		std::thread* Thread;
	private:
		bool UsedStaticPurchase;//静态验证
		bool UsedAdminPassword;
		bool UsedDynamicPurchase;//动态验证
		time_t DynamicEndTime;
		int Days;
		std::string AdminPassword;
		std::string Key;
		std::string HmiId;
		std::vector<StaticHirePurchaseSet*> StaticHirePurchaseSetQueue;
		//int DynamicStartTime;
	};
}


