#pragma once
#include "AdvanceActionMode.h"
#include "BtnFunctionRes.h"
namespace Project
{
	struct ExecutionAction
	{
		bool UsePopWindow;				//�Ƿ����õ�������
		int WindowNumber;				//�л����ڱ��
		bool UseCallMacros;				//�Ƿ���ú���
		char MacroName[128] = { 0 };	//���ú�������
		
		void Parse(rapidjson::Value& jsonObj);
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(UsePopWindow, WindowNumber, UseCallMacros, MacroName);
		}
	};

	union AdvFuncSetup
	{
		struct CommunicationSetting			//ͨ������
		{
			ExecutionAction TimeoutSetting;		//��ʱ����
			int PopWinTimeoutCounts;			//��ʱ������������
			int CallMacroTimeoutCounts;			//��ʱ�������������ã�

			ExecutionAction ErrorSetting;		//��������
			int PopWinErrorCounts;				//���������������
			int CallMacroErrorCounts;			//����������������ã�

			bool HideCell;						//ͨѶʧ�ܺ��Ƿ���ʾ����ͨѶ�豸Ԫ��
			
			void Parse(rapidjson::Value& jsonObj);
			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(TimeoutSetting, PopWinTimeoutCounts, CallMacroTimeoutCounts,
					ErrorSetting, PopWinErrorCounts, CallMacroErrorCounts, HideCell);
			}
		}CommunicationSetting;
		struct FlashDiskSetting				//U������
		{
			ExecutionAction FullMem;		//�ڴ治������
			bool TipUpdate;					//�Ƿ���ʾ���³���

			void Parse(rapidjson::Value& jsonObj);
			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(FullMem, TipUpdate);
			}

		}FlashDiskSetting;
		struct SDCardSetting				//SD������
		{
			ExecutionAction FullMem;		//�ڴ治������
			bool TipUpdate;					//�Ƿ���ʾ���³���

			void Parse(rapidjson::Value& jsonObj);
			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(FullMem, TipUpdate);
			}
		}SDCardSetting;
		struct SystemParam
		{
			ExecutionAction FullMem;		//�ڴ治������
			ExecutionAction CpuOvertop;		//CPU��������

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
			WinAction CallWindowMode;		//0:�л�����	1:��������		2:������һ����
			int WindowNumber;				//�л����ڱ��
			bool IsShowPasswordWindow;		//�Ƿ񵯳����봰��
			
			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(CallWindowMode, WindowNumber, IsShowPasswordWindow);
			}


		}CallWindow;
		struct CloseWindow
		{
			int CloseWindowMode;				//0:�ر����д���	1:�ر�ָ������
			int AssignCloseWindowNumber;		//ָ�����ڱ��

			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(CloseWindowMode, AssignCloseWindowNumber);
			}
		}CloseWindow;
		struct DataTransfer					//���ݴ���
		{
			int SendType;					//0:�� 1:λ
			int RegNum;						//�Ĵ�������
			DataVarId SrcVar;				//Դ�Ĵ���
			DataVarId DstVar;				//Ŀ�ļĴ���

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

