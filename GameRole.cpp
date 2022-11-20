#include "GameRole.h"
#include"GameMsg.h"
#include"GameProto.h"
#include"msg.pb.h"
#include"GameChannel.h"
#include <iostream>

static AOIWorld world(0,400,0,400,20,20);

GameMsg* GameRole::CreateIDNameLogin()
{
    pb::SyncPid* pSync = new pb::SyncPid();
    pSync->set_pid(Pid);
    pSync->set_username(Player_Name);

    GameMsg* Login_Msg = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pSync);
    return Login_Msg;
}

GameRole::GameRole()
{
    x = 100; 
    z = 100;
    Player_Name ="Tom";
}

GameRole::~GameRole()
{
}

bool GameRole::Init()
{
    /*设置玩家ID为当前连接的fd*/
    //由getfd来实现id的唯一性
    //不能在构造函数中初始话，因为那里还没有连接Ro_proto        
    Pid = this->Ro_proto->m_channel->GetFd();

    bool returnValue = false;

    //把自己这个role添加到world中
    returnValue = world.AddPlayerS(this);

    if (returnValue == true)
    {
        auto loginMsg = CreateIDNameLogin();
        ZinxKernel::Zinx_SendOut(*loginMsg,*Ro_proto);
    }


    return returnValue;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    //测试proto处理的数据是否发给role
   /* GET_REF2DATA(MultiMsg, Mesg, _poUserData);
    for (auto task : Mesg.m_Msg)
    {
        std::cout <<"type is"<< task->enMsgType << std::endl;
        std::cout << task->pMsg->Utf8DebugString() << std::endl;
    }*/
    return nullptr;
}

void GameRole::Fini()
{
    world.DelPlayers(this);
}

int GameRole::getX()
{
    return (int)x;
}

int GameRole::getY()
{
    return (int)z;
}
