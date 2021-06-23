#pragma once
#include "AdvanceActionMode.h"
#include "BtnFunctionRes.h"
namespace Project
{
	struct ExecutionAction
	{
		bool UsePopWindow;				//是否启用弹窗设置
		int WindowNumber;				//切换窗口编号
		bool UseCallMacros;				//是否调用函数
		char MacroName[128] = { 0 };	//调用函数名称
		
		void Parse(rapidjson::Value& jsonObj);
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(UsePopWindow, WindowNumber, UseCallMacros, MacroName);
		}
	};

	union AdvFuncSetup
	{
		struct CommunicationSetting			//通信设置
		{
			ExecutionAction TimeoutSetting;		//超时设置
			int PopWinTimeoutCounts;			//超时次数（弹窗）
			int CallMacroTimeoutCounts;			//超时次数（函数调用）

			ExecutionAction ErrorSetting;		//报错设置
			int PopWinErrorCounts;				//报错次数（弹窗）
			int CallMacroErrorCounts;			//报错次数（函数调用）

			bool HideCell;						//通讯失败后，是否不显示对于通讯设备元件
			
			void Parse(rapidjson::Value& jsonObj);
			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(TimeoutSetting, PopWinTimeoutCounts, CallMacroTimeoutCounts,
					ErrorSetting, PopWinErrorCounts, CallMacroErrorCounts, HideCell);
			}
		}CommunicationSetting;
		struct FlashDiskSetting				//U盘设置
		{
			ExecutionAction FullMem;		//内存不足设置
			bool TipUpdate;					//是否提示更新程序

			void Parse(rapidjson::Value& jsonObj);
			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(FullMem, TipUpdate);
			}

		}FlashDiskSetting;
		struct SDCardSetting				//SD卡设置
		{
			ExecutionAction FullMem;		//内存不足设置
			bool TipUpdate;					//是否提示更新程序

			void Parse(rapidjson::Value& jsonObj);
			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(FullMem, TipUpdate);
			}
		}SDCardSetting;
		struct SystemParam
		{
			ExecutionAction FullMem;		//内存不足设置
			ExecutionAction CpuOvertop;		//CPU过高设置

			void Parse(rapidjson::Value& jsonObj);
			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(FullMem, CpuOvertop);
			}

		}SystemParam;
		struct Restart
		{
			int Mode;
			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(Mode);
			}

		}Restart;
		struct CallMacros
		{
			char MacroName[128] = { 0 };

			void Parse(rapidjson::Value& jsonObj);
			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(MacroName);
			}

		}CallMacros;
		struct CallWindow
		{
			WinAction CallWindowMode;		//0:切换窗口	1:弹出窗口		2:返回上一窗口
			int WindowNumber;				//切换窗口编号
			bool IsShowPasswordWindow;		//是否弹出密码窗口
			
			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(CallWindowMode, WindowNumber, IsShowPasswordWindow);
			}


		}CallWindow;
		struct CloseWindow
		{
			int CloseWindowMode;				//0:关闭所有窗口	1:关闭指定窗口
			int AssignCloseWindowNumber;		//指定窗口编号

			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(CloseWindowMode, AssignCloseWindowNumber);
			}
		}CloseWindow;
		struct DataTransfer					//数据传输
		{
			int SendType;					//0:字 1:位
			int RegNum;						//寄存器数量
			DataVarId SrcVar;				//源寄存器
			DataVarId DstVar;				//目的寄存器

			void Parse(rapidjson::Value& jsonObj);
			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(SendType, RegNum, SrcVar, DstVar);
			}
		}DataTransfer;
		struct CallSystemCommand
		{
			char CommandName[128] = { 0 };

			void Parse(rapidjson::Value& jsonObj);
			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(CommandName);
			}
		}CallSystemCommand;

		AdvFuncSetup() {};
		~AdvFuncSetup() {};

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(CommunicationSetting, FlashDiskSetting, SDCardSetting, SystemParam,
				Restart, CallMacros, CallWindow, CloseWindow, DataTransfer, CallSystemCommand);
		}
	};

	struct AdvanceFuncRes :public AdvanceActionMode
	{
		std::string FunctionName;
		AdvFuncSetup FunctionParam;
		static void Parse(std::vector<AdvanceFuncRes>& vector, rapidjson::Value& jsonObj);
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(FunctionName, FunctionParam, Trigmode, BitTrigCondition, 
				WordTrigCondition, WordTrigValue, TrigDatavrIdRef);
		}
	};
}

