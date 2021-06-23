#pragma once

class IHMIThread
{
public:
	virtual void Run()=0;
	virtual void Exit() = 0;
};
