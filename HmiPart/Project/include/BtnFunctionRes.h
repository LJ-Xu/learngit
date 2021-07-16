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
		AddRecipe,			//添加
		InsertRecipe,		//插入
		DeleteRecipe,		//删除
		CopyRecipe,			//复制
		MoveUpRecipe,		//上移
		MoveDownRecipe		//下移
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
			DataAction ActionData;			//操作（递增递减只在按下状态）
			double Value;					//设置数值
			DataVarId ValueVar;				//数值读取寄存器
			DataVarId WriteVarIdRef;		//写入寄存器		
			//double WriteValue;				//写入寄存器初始值
			//bool FirstHandel = true;
			double IncreaseValue;			//递增或递减值
			DataVarId IncreaseValueVar;
			double UpperLmtValue;
			DataVarId UpperLmtValueVar;
			double LowerLmtValue;
			DataVarId LowerLmtValueVar;
			bool Loop;						//是否周期循环
			float Rate;						//循环周期（只在递增递减处使用）

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

			MathAction ActionMath;			//运算方法
			DataVarId WriteVarRef;			//写入地址

			Operand Left;					//左值
			Operand Right;					//又值
			bool UpperUsed;					//是否使用上限
			double UpperLmtValue;			//上限
			DataVarId UpperLmtValueVar;
			bool LowerUsed;					//是否使用下限
			double LowerLmtValue;			//下限
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
			int SendType;		//0:字 1:位
			int RegNum;			//寄存器数量
			DataVarId SrcVar;	//源寄存器
			DataVarId DstVar;	//目的寄存器

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
			WinAction ActionWin;	//调用窗口方式
			int ExchangeWinNo;		//切换窗口编号
			int PopUpWinNo;			//弹出窗口编号
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
			int CloseAction;		//0:关闭所有窗口	1:关闭指定窗口
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
			
			DataVarId OrgVarIdRef;		//数据源地址
			FlieLocation FileAddr;		//目标文件位置
			DataVarId LocationVar;		//动态指定文件位置

			char FileName[128] = {};
			//std::string FileName;		//文件名称
			FlieNameType NameType;		//文件名称方式
			DataVarId NameVar;			//动态指定文件名获取地址

			int Count;					//数据容量
			int DataSize;				//列数
			DataContent Data[100] = {};

			DataVarId StatusVarRef;		//执行状态
			DataVarId ResultVarRef;		//执行结果
			DataVarId ProgressVarRef;	//执行过程

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
			DataVarId OrgVarIdRef;		//数据源地址
			FlieLocation FileAddr;		//目标文件位置
			DataVarId LocationVar;		//动态指定文件位置
			//std::string FileName;		//文件名称
			char FileName[128] = { 0 };
			FlieNameType NameType;		//文件名称方式
			DataVarId NameVar;			//动态指定文件名获取地址

			int Count;				//数据容量
			int DataSize;				//列数
			DataContent Data[100];

			DataVarId StatusVarRef;		//执行状态
			DataVarId ResultVarRef;		//执行结果
			DataVarId ProgressVarRef;	//执行过程

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
			WinSrc ScreenSrc;	//打印窗口来源
			DataVarId ScreenVarIdRef;	//读取地址
			int WinNo;			//窗口编号
			int StorePos;		//存储位置
			int PrintDir;		//0:水平	1:垂直
			int PrintMode;		//打印模式
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
		bool PopTipWin;			//是否弹窗
		int RequireRegion;		//用户权限
		static void Parse(std::vector<BtnFunctionRes>& vector, rapidjson::Value& jsonObj);

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(FunctionName, FunctionParam, PopTipWin, RequireRegion);
		}
	}; 
	
}

