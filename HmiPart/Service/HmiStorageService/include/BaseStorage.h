#pragma once
#include "XJRingBuffer.h"
namespace Storage
{
	enum ResultCode
	{
		SS_SUCCESS,
		SS_FAIL
	};
	struct StorageConfig
	{
		int  Capacity; //Ԫ�صĸ���
		int  ItemSize;//����Ԫ�صĴ�С
		int  SaveLevel;
		char Path[20];//����·��
		char Media;//�洢����
		char Days;//�����ʱ������
	};
	
	class BaseStorage :public IObserver
	{
	public :
		void Init(StorageConfig& cnf);
		void Clear();
		void Save();
		void StorageNotify(int count);
		virtual void Load() = 0;//���ö�ָ��
	protected:
		virtual int UpdateCurse(int count)=0;//���ö�ָ��
		virtual int GetCount() = 0;//���ö�ָ��
		virtual char* operator[](int idx)=0;
		StorageConfig cnf_;
	private:
		
	};
 
}
