#pragma once
#include <string>
#include <vector>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "DataVarInfo.h"

namespace Project
{
	//void run(std::string name, std::vector<char> data)
	//{
	//	//name json
	//	/*if (cacheFuncMap.count(name)==0)*/
	//	{
	//		std::shared_ptr<BaseBtnFunction> func = newFuncMap[name]();
	//		func->Parse(data);
	//		func->Handle();
	//		//cacheFuncMap.insert(std::pair<std::string, std::shared_ptr<BaseFunc>>(name, func));
	//	/*}
	//	else
	//	{
	//		cacheFuncMap[name]->Handle();
	//	}*/
	//	}
	//}
	
	//static std::map<FunctionName, std::shared_ptr<BaseBtnFunction>(*)(void)> newFuncMap = {
	//	{FunctionName::SetCoilFunc,[]() {return NewFunc<SetCoil>(); }},
	//	{FunctionName::SendDataFunc,[]() {return NewFunc<SetData>(); }},
	//	{FunctionName::ArithmeticFunc,[]() {return NewFunc<Arithmetic>(); }},
	//	{FunctionName::SendDataFunc,[]() {return NewFunc<SendData>(); }},
	//	{FunctionName::SwitchScreenFunc,[]() {return NewFunc<SwitchScreen>(); }},
	//	{FunctionName::OpenWinFunc,[]() {return NewFunc<OpenWin>(); }},
	//	{FunctionName::CloseWinFunc,[]() {return NewFunc<CloseWin>(); }},
	//	{FunctionName::ImportCSVdataFunc,[]() {return NewFunc<ImportCSVdata>(); }},
	//	{FunctionName::ExportCSVdataFunc,[]() {return NewFunc<ExportCSVdata>(); }},
	//	{FunctionName::UploadRecipeFunc,[]() {return NewFunc<UploadRecipe>(); }},
	//	{FunctionName::DownloadRecipeFunc,[]() {return NewFunc<DownloadRecipe>(); }},
	//	{FunctionName::FuncCallback,[]() {return NewFunc<CallbackFunc>(); }},
	//	{FunctionName::PrintScreenFunc,[]() {return NewFunc<PrintScreen>(); }},
	//};

	enum CoilAction:char { SETON, SETOFF, OPPOSITE };
	enum DataAction :char { CONSTANT, ADD, MINUS, INCREASE, DECREASE };
	enum MathAction :char { ADDTION, SUB, MULTIPLY, DIVISION };
	enum FrameAction :char { START, FRONT, NUM };
	enum WinAction :char { SWITCH, OPEN, RETURN };
	enum FlieLocation :char { USB, SD, HMI, FileREG };
	enum FlieNameType :char { FIX, DATA, DYNAMIC };
	enum WinSrc :char { CURRENT, REGVAR, WINNO };

	enum RecipeEditAction:char{
		AddRecipe,			//���
		InsertRecipe,		//����
		DeleteRecipe,		//ɾ��
		CopyRecipe,			//����
		MoveUpRecipe,		//����
		MoveDownRecipe		//����
	};
	struct DataContent
	{
		int Id;
		//std::string Title;
		char Title[128] = { 0 };

		int DataType;
		int DataFmt;
		int Number;
		int IntegerNum;
		int DecimalNum;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Id, Title, DataType, DataFmt, Number, IntegerNum, DecimalNum);
		}
	};
	union FunctionSetup
	{
		struct SetCoil
		{
			CoilAction ActionCoil;
			DataVarId RegVar;
			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(ActionCoil, RegVar);
			}
		}SetCoil;
		struct SetData
		{
			DataAction ActionData;			//�����������ݼ�ֻ�ڰ���״̬��
			double Value;					//������ֵ
			DataVarId ValueVar;				//��ֵ��ȡ�Ĵ���
			DataVarId WriteVarIdRef;		//д��Ĵ���		
			//double WriteValue;				//д��Ĵ�����ʼֵ
			//bool FirstHandel = true;
			double IncreaseValue;			//������ݼ�ֵ
			DataVarId IncreaseValueVar;
			double UpperLmtValue;
			DataVarId UpperLmtValueVar;
			double LowerLmtValue;
			DataVarId LowerLmtValueVar;
			bool Loop;						//�Ƿ�����ѭ��
			float Rate;						//ѭ�����ڣ�ֻ�ڵ����ݼ���ʹ�ã�

			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(ActionData, WriteVarIdRef, Value, ValueVar, IncreaseValue, IncreaseValueVar,
					UpperLmtValue, UpperLmtValueVar, LowerLmtValue, LowerLmtValueVar, Loop, Rate);
			}

		}SetData;
		struct Arithmetic 
		{
			enum MathDataType :char
			{
				BCD_16,
				BCD_32,
				BCD_64,
				Hex_16,
				Hex_32,
				Hex_64,
				Sigend_16,
				Sigend_32,
				Sigend_64,
				Unsigend_16,
				Unsigend_32,
				Unsigend_64,
				Float_32,
				Float_64
			};
			struct Operand
			{
				MathDataType Type;
				double Value;
				DataVarId ValueVar;
				template<class Archive>
				void serialize(Archive & archive)
				{
					archive(Type, Value, ValueVar);
				}
			};

			MathAction ActionMath;			//���㷽��
			DataVarId WriteVarRef;			//д���ַ

			Operand Left;					//��ֵ
			Operand Right;					//��ֵ
			bool UpperUsed;					//�Ƿ�ʹ������
			double UpperLmtValue;			//����
			DataVarId UpperLmtValueVar;
			bool LowerUsed;					//�Ƿ�ʹ������
			double LowerLmtValue;			//����
			DataVarId LowerLmtValueVar;

			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(ActionMath, WriteVarRef, Left, Right, UpperUsed,
					UpperLmtValue, UpperLmtValueVar, LowerUsed,
					LowerLmtValue, LowerLmtValueVar);
			}
		}Arithmetic;
		struct SendData
		{
			int SendType;		//0:�� 1:λ
			int RegNum;			//�Ĵ�������
			DataVarId SrcVar;	//Դ�Ĵ���
			DataVarId DstVar;	//Ŀ�ļĴ���

			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(SendType, RegNum, SrcVar, DstVar);
			}

		}SendData;
		struct SwitchScreen
		{
			FrameAction Frame;
			int FrameNum;
			bool UsePasswd;

			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(Frame, FrameNum, UsePasswd);
			}

		}SwitchScreen;
		struct OpenWin
		{
			WinAction ActionWin;	//���ô��ڷ�ʽ
			int ExchangeWinNo;		//�л����ڱ��
			int PopUpWinNo;			//�������ڱ��
			bool UsePasswd;

			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(ActionWin, ExchangeWinNo, PopUpWinNo, UsePasswd);
			}


		}OpenWin;
		struct CloseWin 
		{
			int CloseAction;		//0:�ر����д���	1:�ر�ָ������
			int CloseNum;
			bool UsePasswd;

			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(CloseAction, CloseNum, UsePasswd);
			}
		}CloseWin;
		struct ImportCSVdata 
		{
			
			DataVarId OrgVarIdRef;		//����Դ��ַ
			FlieLocation FileAddr;		//Ŀ���ļ�λ��
			DataVarId LocationVar;		//��ָ̬���ļ�λ��

			char FileName[128] = {};
			//std::string FileName;		//�ļ�����
			FlieNameType NameType;		//�ļ����Ʒ�ʽ
			DataVarId NameVar;			//��ָ̬���ļ�����ȡ��ַ

			int Count;					//��������
			int DataSize;				//����
			DataContent Data[100] = {};

			DataVarId StatusVarRef;		//ִ��״̬
			DataVarId ResultVarRef;		//ִ�н��
			DataVarId ProgressVarRef;	//ִ�й���

			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(FileAddr, LocationVar, FileName, NameType, 
					NameVar, OrgVarIdRef, Count, Data, DataSize,
					StatusVarRef, ResultVarRef, ProgressVarRef);
			}
		}ImportCSVdata;
		struct ExportCSVdata
		{
			DataVarId OrgVarIdRef;		//����Դ��ַ
			FlieLocation FileAddr;		//Ŀ���ļ�λ��
			DataVarId LocationVar;		//��ָ̬���ļ�λ��
			//std::string FileName;		//�ļ�����
			char FileName[128] = { 0 };
			FlieNameType NameType;		//�ļ����Ʒ�ʽ
			DataVarId NameVar;			//��ָ̬���ļ�����ȡ��ַ

			int Count;				//��������
			int DataSize;				//����
			DataContent Data[100];

			DataVarId StatusVarRef;		//ִ��״̬
			DataVarId ResultVarRef;		//ִ�н��
			DataVarId ProgressVarRef;	//ִ�й���

			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(FileAddr, LocationVar, FileName, NameType, 
					NameVar, OrgVarIdRef, Count, Data, DataSize,
					StatusVarRef, ResultVarRef, ProgressVarRef);
			}
		}ExportCSVdata;
		struct DownloadRecipe 
		{
			char RecipeName[128] = { 0 };
			int RecipeRow;
			int Size;
			DataVarId RegVar;
			DataVarId TransferVarIdRef;

			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(RecipeName, Size, RegVar, TransferVarIdRef);
			}
		}DownloadRecipe;
		struct UploadRecipe
		{
			char RecipeName[128] = { 0 };
			int RecipeRow;

			int Size;
			DataVarId RegVar;
			DataVarId TransferVarIdRef;

			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(RecipeName, Size, RegVar, TransferVarIdRef);
			}

		}UploadRecipe;
		struct CallbackFunc
		{
			//std::string FunctionName;
			char CbFuncName[128] = {0};
			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(CbFuncName);
			}

		}CallbackFunc;
		struct PrintScreen
		{
			WinSrc ScreenSrc;	//��ӡ������Դ
			DataVarId ScreenVarIdRef;	//��ȡ��ַ
			int WinNo;			//���ڱ��
			int StorePos;		//�洢λ��
			int PrintDir;		//0:ˮƽ	1:��ֱ
			int PrintMode;		//��ӡģʽ
			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(ScreenSrc, ScreenVarIdRef, WinNo, PrintDir, StorePos, PrintMode);
			}

		}PrintScreen;
		struct EditRecipe
		{
			RecipeEditAction Action;
			void Parse(rapidjson::Value& jsonObj);

			template<class Archive>
			void serialize(Archive & archive)
			{
				archive(Action);
			}
		}EditRecipe;
		FunctionSetup() {};
		~FunctionSetup() {};

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(SetCoil, SetData, Arithmetic, SendData, SwitchScreen,
				OpenWin, CloseWin, ImportCSVdata, ExportCSVdata, DownloadRecipe,
				UploadRecipe, CallbackFunc, PrintScreen, EditRecipe);
		}
	};
	struct BtnFunctionRes
	{
		std::string FunctionName;
		FunctionSetup FunctionParam;
		bool PopTipWin;			//�Ƿ񵯴�
		int RequireRegion;		//�û�Ȩ��
		static void Parse(std::vector<BtnFunctionRes>& vector, rapidjson::Value& jsonObj);

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(FunctionName, FunctionParam, PopTipWin, RequireRegion);
		}
	}; 
	
}

