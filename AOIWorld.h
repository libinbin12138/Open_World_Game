#pragma once
#include<list>
#include<vector>
class Player
{
public:
	virtual int getX() = 0;
	virtual int getY() = 0;
};

class Grid//Иёзг
{
public:
	std::list<Player*>m_player;
};

class AOIWorld
{
	int x_begin = 0;
	int x_end = 0;
	int y_begin = 0;
	int y_end = 0;
	int x_count = 0;
	int y_count = 0;
	int x_width = 0;
	int y_width = 0;
public:
	std ::vector<Grid>m_grids;

	AOIWorld(int _x_begin, int _x_end, int _y_begin, 
		int  _y_end, int _x_count, int _y_count);

	std::list<Player*>GetRoundPlayer(Player*player);
	bool AddPlayerS(Player* player);
	void DelPlayers(Player* player);
};

