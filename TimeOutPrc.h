#pragma once
#include<zinx.h>
#include<list>
#include<vector>
using namespace std;
class TimeOutPrc
{
public:
	virtual void prc()=0;
	virtual int gettimer() = 0;
	int timeOut = -1;
};

class TimeOutMsg :public AZinxHandler
{
	std::list<TimeOutPrc*>task_list;
	static TimeOutMsg sigle;
public:
	// 通过 AZinxHandler 继承
	virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
	virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;
	static TimeOutMsg & getInstance()
	{
		return sigle;
	}
	void addTimeOutPrc(TimeOutPrc* _prc);
	void delTimeOutPrc(TimeOutPrc* _prc);
};

class TimeOutWheel :public AZinxHandler
{
	TimeOutWheel();
	vector <list<TimeOutPrc*>> v_wheel;
	static TimeOutWheel single;
	int cur_mark = 0;

public:
	static TimeOutWheel& getInstance()
	{
		return single;
	}
	// 通过 AZinxHandler 继承
	virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
	virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;
	void addTimeOutPrc(TimeOutPrc* _prc);
	void delTimeOutPrc(TimeOutPrc* _prc);
};