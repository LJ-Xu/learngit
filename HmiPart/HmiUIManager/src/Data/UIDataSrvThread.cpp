#include "DataClient.h"
#include "HMISocket.h"
#include "UIDataSrvThread.h"
#include "ClientSession.h"
#include "DataClient.h"
namespace UI
{
	 

	/*UIDataSrvThread& UIDataSrvThread::Ins()
	{
		static UIDataSrvThread ins;
		return ins;
	}*/
	/*UIDataSrvThread::UIDataSrvThread()
	{
		
	}*/
	void UIDataSrvThread::Init(Project::ProjectPortVarsInfo* ppvi, HMI::IDaAppCB *appcb)
	{
		ppvi_ = ppvi;
		client_ = new  HMI::DataClient();
		client_->Init();
		client_->SetCB(appcb);
		
		dataFrame_ = new UIDataFrame();
		dataFrame_->Init(ppvi);


		
		
	}
	void UIDataSrvThread::RecvRun(UIDataSrvThread* self)
	{

		
		//while (self->runFlag_)
		//{
		//	//self->client_->Connect();
		//	while (self->runFlag_)
		//	{

		//		try
		//		{
		//			if (self->client_->Recv() == 0)
		//				break;
		//		}
		//		catch (const SocketException& exception)
		//		{
		//			self->client_ = nullptr;
		//			//const wchar_t*  str = exception.wwhat();
		//			std::cout << exception.wwhat() << L'\n';

		//			break;
		//		}
		//	}

		//}
	}
	void UIDataSrvThread::SendRun(UIDataSrvThread* self)
	{
		DataRequest dr;
		int cnt = 0;
		while (self->runFlag_)
		{
			while (!self->client_->GetSession())
				Sleep(1);
			//self->client_->Connect();
			while (self->runFlag_)
			{
				if (self->dataFrame_->Get(dr)==0)
				{
					try
					{
						self->LockSend();
						self->client_->GetSession()->Read(dr.VarPtr, dr.type, dr.id, dr.eid);
						self->UnLockSend();
						
							
					}
					catch (...)
					{
						/*std::cout << exception.wwhat() << L'\n';*/
						break;
					}
					if (cnt++ == 20)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					
						cnt = 0;
					}
					//std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				else
				{
					//没有可用数据
					//std::this_thread::sleep_for(std::chrono::milliseconds(1));
					cnt = 0;
				}
			}
		}
	}
	void UIDataSrvThread::Run()
	{
		runFlag_ = true;
		if(!client_->Connect("127.0.0.1",8800))
		{
			//recvTd_ = new thread(RecvRun, this);
			sendTd_ = new thread(SendRun, this);
		}
		
	}
	void UIDataSrvThread::Exit()
	{
		runFlag_ = false;
	}
	

  
	bool UIDataSrvThread::PostRead(Project::DataVarId vid)
	{
		Project::BaseVar* var = ppvi_->GetDataVarInfo(vid);
		LockSend();
		client_->GetSession()->ReadByVar((BaseVar*)var, vid.Vid);
		UnLockSend();
		return true;
	}

	bool UIDataSrvThread::PostRead(Project::DataVarId vid, int count)
	{
		Project::BaseVar tmp;
		Project::BaseVar* var = ppvi_->GetDataVarInfo(vid);
		tmp = *var;
		tmp.Count = count;
		LockSend();
		client_->GetSession()->ReadByVar((BaseVar*)&tmp, vid.Vid);
		UnLockSend();
		return true;
	}
	bool UIDataSrvThread::PostRead(int vid,Project::BaseVar& var)
	{
		LockSend();
		client_->GetSession()->ReadByVar((BaseVar*)&var, vid);
		UnLockSend();
		return true;
	}
	bool UIDataSrvThread::MacroTrigger(const char* funcname)
	{
		LockSend();
		client_->GetSession()->TriggerMacro(funcname);
		UnLockSend();
		return true;
	}

	bool UIDataSrvThread::PostWrite(Project::BaseVar& var, const BYTE* data, int len)
	{
		LockSend();
		client_->GetSession()->WriteByVar((BaseVar*)&var, (char*)data, len);
		UnLockSend();
		return true;
	}
 
	bool UIDataSrvThread::PostWrite(Project::DataVarId& vid,const BYTE* data, int len)
	{
		Project::BaseVar* var = ppvi_->GetDataVarInfo(vid);
		LockSend();
		client_->GetSession()->WriteByVar((BaseVar*)var, (char*)data, len);
		UnLockSend();
		return true;
	}


	void UIDataSrvThread::AddWin(int winno)
	{
		dataFrame_->AddWin(winno);
	}
	void UIDataSrvThread::RemoveWin(int winno)
	{
		dataFrame_->RemoveWin(winno);
	}

	
}

