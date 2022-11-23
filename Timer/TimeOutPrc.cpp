#include "TimeOutPrc.h"
using namespace std;

TimeOutMsg TimeOutMsg::sigle;
TimeOutWheel TimeOutWheel::single;


IZinxMsg* TimeOutMsg::InternelHandle(IZinxMsg& _oInput)
{
	for (auto task : task_list)
	{
		task->timeOut--;
		if (task->timeOut <= 0)
		{
			task->prc();
			task->timeOut = task->gettimer();
		}
	}
	return nullptr;
}

AZinxHandler* TimeOutMsg::GetNextHandler(IZinxMsg& _oNextMsg)
{
	return nullptr;
}

void TimeOutMsg::addTimeOutPrc(TimeOutPrc* _prc)
{
	this->task_list.push_back(_prc);
	_prc->timeOut = _prc->gettimer();
}


void TimeOutMsg::delTimeOutPrc(TimeOutPrc* _prc)
{
	this->task_list.remove(_prc);
}





TimeOutWheel::TimeOutWheel()
{
	for (int i = 0; i < 10; i++)
	{
		list<TimeOutPrc*>temp;
		this->v_wheel.push_back(temp);
	}

}

IZinxMsg* TimeOutWheel::InternelHandle(IZinxMsg& _oInput)
{
	unsigned long timeout;
	GET_REF2DATA(BytesMsg, obytes, _oInput);
	obytes.szData.copy((char*)&timeout, sizeof(timeout), 0);
	while (timeout-->0)//Ϊʲô��---��
	{
		cur_mark++;
		cur_mark %= 10;
		list<TimeOutPrc*>L_save;
		for (auto itr = this->v_wheel[cur_mark].begin(); itr != this->v_wheel[cur_mark].end();)//list<TimeOutPrc*>::iterator)
		{
			if ((*itr)->timeOut <= 0)
			{
				//(*itr)->prc();
				L_save.push_back(*itr);//�Ĵ���˼·
				auto temp = *itr;
				itr=v_wheel[cur_mark].erase(itr);//list������erase�󷵻���һ���ڵ��ַ
				this->addTimeOutPrc(temp);

			}
			else
			{
				(*itr)->timeOut--;
				++itr;

			}
		}

		for (auto task : L_save)
		{
			task->prc();
		}
	}
	return nullptr;
}

AZinxHandler* TimeOutWheel::GetNextHandler(IZinxMsg& _oNextMsg)
{
	return nullptr;
}

void TimeOutWheel::addTimeOutPrc(TimeOutPrc* _prc)
{
	int idex = (_prc->gettimer() + this->cur_mark) % 10;
	_prc->timeOut = _prc->gettimer() / 10;

	this->v_wheel[idex].push_back(_prc);//task_list.push_back(_prc);
}
	

void TimeOutWheel::delTimeOutPrc(TimeOutPrc* _prc)
{
	for (list<TimeOutPrc*> &chi : v_wheel)//����chi�����ݻḴ��һ�ݸ�v_wheel���������õĻ�v_wheel���cur_task���ᱻɾ��
	{
		for (auto cur_task : chi)
		{
			if (cur_task= _prc)
			{
				chi.remove(cur_task);
				return;
			}
		}
	}


}
