#pragma once
#include<vector>
#include<string>
class NameStructure
{
public:
	std::string m_first;
	std::vector<std::string>m_last_list;
};
class RandomName
{
	std::vector<NameStructure*>Name_Pool;
public:
	RandomName();
	virtual ~RandomName();

	void LodaFile();
	std::string GetName();
	void ReleaseName(std::string _name);
};

