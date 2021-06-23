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
		//��ǰ��֤ģʽ(0��̬1��̬)
		bool IsStatic() {
			return UsedStaticPurchase;
		}
		bool IsDynamic() {
			return UsedDynamicPurchase;
		}
		//�����ַ����������̶�����"2021/5/27"+"19:36:53"�ĸ�ʽ
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
		//��ʼ��һ�ֵ���ʱ�������������ڵ�ʱ����������
		void OnSpanner();
		//�뵱ǰʱ��Ƚ�
		//����0ʱ��ȥʱ(��Ҫ����֤)��1ʱδ��ʱ
		int CmpTm(time_t time);
	private:
		bool isRunning;
		bool Verifiyed;//�Ƿ�����֤��
		std::thread* Thread;
	private:
		bool UsedStaticPurchase;//��̬��֤
		bool UsedAdminPassword;
		bool UsedDynamicPurchase;//��̬��֤
		time_t DynamicEndTime;
		int Days;
		std::string AdminPassword;
		std::string Key;
		std::string HmiId;
		std::vector<StaticHirePurchaseSet*> StaticHirePurchaseSetQueue;
		//int DynamicStartTime;
	};
}


