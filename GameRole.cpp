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
    /*�������IDΪ��ǰ���ӵ�fd*/
    //��getfd��ʵ��id��Ψһ��
    //�����ڹ��캯���г�ʼ������Ϊ���ﻹû������Ro_proto        
    Pid = this->Ro_proto->m_channel->GetFd();

    bool returnValue = false;

    //���Լ����role��ӵ�world��
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
    //����proto����������Ƿ񷢸�role
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
