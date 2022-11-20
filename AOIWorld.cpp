#include "AOIWorld.h"
using namespace std;

AOIWorld::AOIWorld(int _x_begin, int _x_end, int _y_begin,
    int _y_end, int _x_count, int _y_count)
    :x_begin(_x_begin), x_end(_x_end), y_begin(_y_begin),
    y_end(_y_end),x_count(_x_count), y_count(_y_count)
{
    x_width = (x_end - x_begin) / x_count;
    y_width = (y_end - y_begin) / y_count;

    for (int i = 0; i < x_count * y_count; i++)
    {
        Grid temp_grid;
        m_grids.push_back(temp_grid);
    }

}

std::list<Player*> AOIWorld::GetRoundPlayer(Player* player)
{
    std::list<Player*>Round_p;
	//计算当前网格横着数和纵着数的个数
	int grid_id = (player->getX() - x_begin) / x_width 
		+ (player->getY() - y_begin) / y_width * x_count;

	int x_index = grid_id % x_count;
	int y_index = grid_id / x_count;

	if (x_index > 0 && y_index > 0)
	{
		list<Player*>& cur_list = m_grids[grid_id - 1 - x_count].m_player;
		Round_p.insert(Round_p.begin(), cur_list.begin(), cur_list.end());
	}
	if (y_index > 0)
	{
		list<Player*>& cur_list = m_grids[grid_id - x_count].m_player;
		Round_p.insert(Round_p.begin(), cur_list.begin(), cur_list.end());
	}
	if (x_index < x_count - 1 && y_index > 0)
	{
		list<Player*>& cur_list = m_grids[grid_id - x_count + 1].m_player;
		Round_p.insert(Round_p.begin(), cur_list.begin(), cur_list.end());
	}
	if (x_index > 0)
	{
		list<Player*>& cur_list = m_grids[grid_id - 1].m_player;
		Round_p.insert(Round_p.begin(), cur_list.begin(), cur_list.end());
	}
	//自己
	list<Player*>& cur_list = m_grids[grid_id].m_player;
	Round_p.insert(Round_p.begin(), cur_list.begin(), cur_list.end());

	if (x_index < x_count - 1)
	{
		list<Player*>& cur_list = m_grids[grid_id + 1].m_player;
		Round_p.insert(Round_p.begin(), cur_list.begin(), cur_list.end());
	}
	if (x_index > 0 && y_index < y_count - 1)
	{
		list<Player*>& cur_list = m_grids[grid_id - 1 + x_count].m_player;
		Round_p.insert(Round_p.begin(), cur_list.begin(), cur_list.end());
	}
	if (y_index < y_count - 1)
	{
		list<Player*>& cur_list = m_grids[grid_id + x_count].m_player;
		Round_p.insert(Round_p.begin(), cur_list.begin(), cur_list.end());
	}
	if (x_index < x_count - 1 && y_index < y_count - 1)
	{
		list<Player*>& cur_list = m_grids[grid_id + 1 + x_count].m_player;
		Round_p.insert(Round_p.begin(), cur_list.begin(), cur_list.end());
	}


    return Round_p;
}

bool AOIWorld::AddPlayerS(Player* player)
{
    /*计算所属网格号*/
    //网格编号=(x-x轴起始坐标)/x轴网格宽度 
    //+(y-y轴起始坐标)/y轴宽度*x轴网格数量
    int grid_id = (player->getX() - x_begin) / x_width 
        + (player->getY() - y_begin) / y_width * x_count;
    this->m_grids[grid_id].m_player.push_back(player);
    return true;
}

void AOIWorld::DelPlayers(Player* player)
{
    int grid_id = (player->getX() - x_begin) / x_width
        + (player->getY() - y_begin) / y_width * x_count;
    this->m_grids[grid_id].m_player.remove(player);
}
