#include "DeleteHelper.h"
#ifdef __linux
#include <stddef.h>
#endif
DeleteHelper::DeleteHelper()
{

}


DeleteHelper::~DeleteHelper()
{
	for (size_t i = 0; i < delDelegates_.size(); i++)
	{
		delDelegates_[i].DelFunc(delDelegates_[i].Param);
	}
}
