#include "CHirePurchase.h"
#include "System.h"
namespace Project
{
	/*void CHirePurchase::Start()
	{
		if (!isRunning)
		{
			Clock = new boost::asio::steady_timer(Tick_Io_Serv);
			isRunning = true;
			OnSpanner();
		}
		
	}*/
	void CHirePurchase::Parse(rapidjson::Value & jsonObj)
	{
		if (jsonObj.IsObject())
		{
			if (jsonObj.HasMember("UsedStaticPurchase"))
				UsedStaticPurchase = jsonObj["UsedStaticPurchase"].GetBool();

			if (jsonObj.HasMember("UsedAdminPassword"))
				UsedAdminPassword = jsonObj["UsedAdminPassword"].GetBool();

			if (jsonObj.HasMember("UsedDynamicPurchase"))
				UsedDynamicPurchase = jsonObj["UsedDynamicPurchase"].GetBool();

			//if (jsonObj.HasMember("DynamicEndTime"))
			//	DynamicEndTime = jsonObj["DynamicEndTime"].GetInt();

			std::string endDynaTime;
			if (jsonObj.HasMember("DynamicEndTime"))
			{
				endDynaTime = jsonObj["DynamicEndTime"].GetString();
				/*DynamicEndTime = GetTime(endDynaTime);*/
			}
				

			if (jsonObj.HasMember("Days"))
				Days = jsonObj["Days"].GetInt();

			if (jsonObj.HasMember("AdminPassword")&&!jsonObj["AdminPassword"].IsNull())
				AdminPassword = jsonObj["AdminPassword"].GetString();

			if (jsonObj.HasMember("HmiId") && !jsonObj["HmiId"].IsNull())
				HmiId = jsonObj["HmiId"].GetString();

			if (jsonObj.HasMember("Key") && !jsonObj["Key"].IsNull())
				Key = jsonObj["Key"].GetString();

			if (jsonObj.HasMember("StaticHirePurchaseSet") && jsonObj["StaticHirePurchaseSet"].IsArray())
			{
				for (size_t i = 0; i < jsonObj["StaticHirePurchaseSet"].Size(); ++i)
				{
					if (!jsonObj["StaticHirePurchaseSet"][i].IsNull())
					{
						StaticHirePurchaseSet* ptr = new StaticHirePurchaseSet();
						if (ptr->Parse(jsonObj["StaticHirePurchaseSet"][i]))
						{
							StaticHirePurchaseSetQueue.push_back(ptr);
						}
						else
						{
							delete ptr;
						}
						
					}
				}
			}
		}
		isRunning = false;
	}
	time_t CHirePurchase::GetTime(const std::string & strDate, const std::string & strTime)
	{
		int year=0, month=0, day=0;
		int hour=0, min=0, sec=0;
		char chDate[20];
		char chTime[10];
		memset(chDate, 0, 20);
		memset(chTime, 0, 10);
		if (strDate != "")
		{
			if (strTime != "")
			{
				if (sscanf(strTime.c_str(), "%d:%d:%d", hour, min, sec) == -1)
				{
					return -1;
				}
				sprintf(chTime, "%d-%d-%d", hour, min, sec);
			}
			if (sscanf(strDate.c_str(), "%d/%d/%d", year, month, day) == -1)
			{
				return -1;
			}
			sprintf(chDate, "%d-%d-%d %s",year,month,day, strTime.c_str());
			
			std::string strDate(chDate);
			return System::GetTimeStampFromDateTimeString(strDate);
		}
		
		return -1;
	}
	bool CHirePurchase::VerifyAdminPassword(const std::string & pwd)
	{
		if (!UsedAdminPassword)
			return false;
		if (0 == AdminPassword.compare(pwd.c_str()))
		{
			Verifiyed = true;
			return true;
		}
		else
			return false;
	}
	bool CHirePurchase::VerifyCurDate(const std::string & pwd)
	{
		StaticHirePurchaseSet* ptrCur = StaticHirePurchaseSetQueue.front();
		if (ptrCur != nullptr)
		{
			if (0 == ptrCur->Password.compare(pwd.c_str()))
			{
				Verifiyed = true;
				return true;
			}
			else
				return false;
		}
		return false;
	}
	void CHirePurchase::OnSpanner()
	{
		Verifiyed = false;
		StaticHirePurchaseSet* pastHirePurchase = nullptr;
		auto itor = StaticHirePurchaseSetQueue.begin();
		//for(auto itor = StaticHirePurchaseSetQueue.begin();itor != StaticHirePurchaseSetQueue.end();++itor)
		while(1)
		{
			if (itor == StaticHirePurchaseSetQueue.end())
			{
				break;
			}
			//当前遍历到的项为过期配置（需要验证）
			if (CmpTm((*itor)->EndTime) == 0)
			{
				//没有更早的过期配置就保存当前并检查下一个
				if (pastHirePurchase == nullptr)
				{
					pastHirePurchase = *itor;

				}
				//有更早的过期配置就丢弃
				else
				{
					delete *(--itor);
					StaticHirePurchaseSetQueue.erase(itor);
					pastHirePurchase = *itor;
				}
				
			}
			//当前遍历到的项为
			else
			{
				
				//st.expires_at();
				/*if(Clock==nullptr)
					Clock = new boost::asio::steady_timer(Tick_Io_Serv);
				Clock->expires_from_now(std::chrono::duration<__int64, std::nano>((*itor)->EndTime - System::GetCurrentTimeStampMs()));
				Clock->async_wait(boost::bind(&CHirePurchase::OnSpanner, this));*/
				break;
			}
			++itor;
		}
		//弹窗

	}
	int CHirePurchase::CmpTm(time_t time)
	{
		return System::GetCurrentTimeStampMs() < time?1:0;
	}
	bool StaticHirePurchaseSet::Parse(rapidjson::Value & jsonObj)
	{
		if (jsonObj.IsObject())
		{
			std::string endDate;
			std::string endTime;
			if (jsonObj.HasMember("EndDate"))
				endDate = jsonObj["EndDate"].GetString();
			else
				return false;
			if (jsonObj.HasMember("EndTime"))
				endTime = jsonObj["EndTime"].GetString();
			else
				return false;
			EndTime = CHirePurchase::GetTime(endDate, endTime);


			if (jsonObj.HasMember("Description") && !jsonObj["Description"].IsNull())
				Description = jsonObj["Description"].GetString();


			if (jsonObj.HasMember("Password") && !jsonObj["Password"].IsNull())
				Password = jsonObj["Password"].GetString();

			return true;
		}
		return false;
	}
}