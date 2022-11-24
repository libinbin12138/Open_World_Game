#include "RandomName.h"
#include<fstream>
#include<iostream>
#include<random>
using namespace  std;

RandomName::RandomName()
{

}

RandomName::~RandomName()
{
}

void RandomName::LodaFile()
{
	ifstream ifs_first("random_first.txt");
	if (!ifs_first.is_open())
	{
		perror("open random_first.txt err");
	}
	ifstream ifs_last("random_last.txt");
	if (!ifs_last.is_open())
	{
		perror("open random_last.txt err");
	}
	string last_NameTemp;
	vector<string>last_NameTemp_List;
	while (getline(ifs_last, last_NameTemp))
	{
		last_NameTemp_List.push_back(last_NameTemp);
	}
	string first_NameTemp;
	while(getline(ifs_first, first_NameTemp))
	{
		auto Name_list = new NameStructure();
		Name_list->m_first = first_NameTemp;
		Name_list->m_last_list = last_NameTemp_List;
		Name_Pool.push_back(Name_list);
	}
	ifs_first.close();
	ifs_last.close();
}
static default_random_engine  random_engine(time(NULL));

std::string RandomName::GetName()
{
	auto num = random_engine() % Name_Pool.size();
	auto num2= random_engine() % (Name_Pool[num]->m_last_list).size();
	auto first = Name_Pool[num]->m_first;
	auto last = Name_Pool[num]->m_last_list[num2];
	//删掉取出的名
	Name_Pool[num]->m_last_list.erase(Name_Pool[num]->m_last_list.begin() + num2);

	//删掉名为0的姓
	if (Name_Pool[num]->m_last_list.size() <= 0)
	{
		delete Name_Pool[num];
		Name_Pool.erase(Name_Pool.begin()+num);
	}
	return first+" "+last;
}

void RandomName::ReleaseName(std::string _name)
{
	auto pos=_name.find(" ");
	auto first = _name.substr(0, pos);
	auto last = _name.substr(pos + 1, _name.size()-pos - 1);

	bool IsFound = false;
	for (auto single : Name_Pool)
	{
		if (first == single->m_first)//当前姓名列表中有姓
		{
			IsFound = true;
			single->m_last_list.push_back(last);//直接把名加回姓中
			break;
		}
	}
	if (!IsFound)//当前姓名列表中没有姓
	{
		auto TempName = new NameStructure;
		TempName->m_first = first;
		TempName->m_last_list.push_back(last);
		Name_Pool.push_back(TempName);
	}

}
