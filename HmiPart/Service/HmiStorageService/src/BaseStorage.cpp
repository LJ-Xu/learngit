#include "BaseStorage.h"
namespace Storage
{

	void BaseStorage::Init(StorageConfig& cnf)
	{
		cnf_ = cnf;
	}
	void BaseStorage::Clear()
	{
	
	}
	void BaseStorage::Save()
	{
		StorageNotify(GetCount());
	}
	void BaseStorage::StorageNotify(int count)
	{
		for (int i = 0; i < count; i++)
		{
			char* data = (*this)[i];		
			//����data,���ݳ���	//cnf_.ItemSize
		}
		UpdateCurse(count);
	}

}

 