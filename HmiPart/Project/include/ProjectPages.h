#pragma once
#ifdef WIN32
#include "CerealCommon.hpp"
#include "Point.h"
#include "ImageResId.h"
#else
#include "ImageResId.h"
#include "CerealCommon.hpp"
#include "Point.h"
#endif // WIN32

//#include "Color.h"
class BaseUnit;
class DataVarInfo;

using namespace std;
namespace Project
{
	
	//struct Point
	//{
	//	int x;
	//	int y;

	//	template<class Archive>
	//	void serialize(Archive & archive)
	//	{
	//		archive(x,y);
	//	}
	//};
	struct Size
	{
		int W;
		int H;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(W, H);
		}
	};
	struct UnitInfo
	{
		string CtrName;//�ؼ�����
		vector<char> Data;//config data;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(CtrName,Data);
		}
		void InitData(string json);
		UnitInfo() {}
		UnitInfo(string name) { CtrName = name; }
	};
	struct TemplateWin
	{
		int TopWinId=-1;
		int BottomWinId=-1;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(TopWinId, BottomWinId);
		}
	};
	struct PageConfig
	{
		bool IsMonopoly;		//�Ƿ�¢��
		bool IsCloseBtn;		//�Ƿ�ʹ�ùرհ�ť
		int WinBgMode;			//���汳�� 0:��ɫ 1:ͼƬ
		ImageResId WinKey;
		int UserLimit;			//�û�Ȩ��
		int ChangeToLimit;		//�ر�ʱ�л�Ȩ��
		bool IsChangeToLimit;	//�ر����Ƿ��л�Ȩ��
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsMonopoly, IsCloseBtn, WinBgMode, WinKey,
				UserLimit, ChangeToLimit, IsChangeToLimit);
		}
	};
	struct PageInfo
	{
		Point Pos;

		string Name;

		int  Id;//ID��

		short Catogray;

		Size Sz;

		bool KeyEnable;

		vector<UnitInfo> Shapes;//��ʾ��Ԫ
		
		int WinColor;

		TemplateWin Template;
		PageConfig PageCfg;
		//std::map<string, DataVarInfo> dataVars;//KEY �� DataVar��Ӧ   @var
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Pos, Name,Id, Sz, KeyEnable, Shapes, Catogray, WinColor, Template, PageCfg);
		}

		void InitData(string json);
	};

	/*
	enum PageType :int
	{
		UserScreen,
		UserWindow,
		TemplateScreen,
		TemplateWindow,
		SysScreen,
		SysWindow,
		PageTypeCount
	};
	static const int offset[PageTypeCount + 1] = { 1,5001,10001,15001,20001,25001,125001 };
	*/


	struct ProjectPages
	{		
		map<int, PageInfo> Screens;//�����ţ�ҳ��

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Screens);
		}
		void InitData(std::string jstr);
	};
}

