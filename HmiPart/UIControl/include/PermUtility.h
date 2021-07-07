#pragma once
#include <string>
#include "Permission.h"
#include "UIComm.h"
namespace UI
{
#define SafeTipsWinNum	25002
#define SafeLoginWinNum 25001
#define SafeConfirmWinNum 25010
#define COMMUFailWinNum	  25011
	enum PremConfirm { OverTime, TriggerOperate, ConfirmOperate, CancelOperate, ConfirmParam};
	class PermUtility
	{
	public:
		static void HandleShowPerm(Project::Permission& perm, HMIBaseView *view);			//������ʾȨ��
		static void HandleEnablePerm(Project::Permission& perm, HMIBaseView *view);		//����ʹ��Ȩ��
		static bool HasUserPerm(int LimitRegion);
		static bool HandleConfirmPerm(Project::Permission& perm, Project::WinCtrlID ctrlid);
		static void QuitLimitPerm(Project::Permission& perm);
	};
}
