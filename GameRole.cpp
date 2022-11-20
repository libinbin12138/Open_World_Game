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

GameMsg* GameRole::CreateRoundPlayer()
{
    pb::SyncPlayers* pRound = new pb::SyncPlayers();
    auto round_list = world.GetRoundPlayer(this);
    for (auto single : round_list)
    {
        auto pPlayer = pRound->add_ps();
        auto pcurRole = dynamic_cast<GameRole*> (single);

        pPlayer->set_username(pcurRole->Player_Name);
        pPlayer->set_pid(pcurRole->Pid);

        auto pPosition = pPlayer->mutable_p();
        pPosition->set_x(pcurRole->x);
        pPosition->set_y(pcurRole->y);
        pPosition->set_z(pcurRole->z);
        pPosition->set_v(pcurRole->v);
    }

    GameMsg* RoundP_MSA = new GameMsg(GameMsg::MSG_TYPE_SRD_POSTION, pRound);
    return RoundP_MSA;
}

GameMsg* GameRole::CreateSelfInfo()
{
    pb::BroadCast* pSelf = new pb::BroadCast();

    pSelf->set_pid(Pid);
    pSelf->set_tp(2);//�Ϳͻ��˶�Ӧ��2���Ƿ����Լ�����Ϣ���Լ�
    pSelf->set_username(Player_Name);

    auto pPosition =pSelf->mutable_p();
    pPosition->set_x(x);
    pPosition->set_y(y);
    pPosition->set_z(z);
    pPosition->set_v(v);

    GameMsg* SelfMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pSelf);

    return SelfMsg;
}

GameMsg* GameRole::CreateIDNameLogoff()
{
    pb::SyncPid* pSync = new pb::SyncPid();
    pSync->set_pid(Pid);
    pSync->set_username(Player_Name);

    GameMsg* Login_Msg = new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID_NAME, pSync);
    return Login_Msg;
}

GameMsg* GameRole::CreateTalkBroadCast(std::string _content)
{
    pb::BroadCast* Chatinfo = new pb::BroadCast();
    Chatinfo->set_pid(Pid);
    Chatinfo->set_username(Player_Name);
    Chatinfo->set_pid(1);//1��������
    Chatinfo->set_content(_content);

    GameMsg* ChatMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, Chatinfo);
    return ChatMsg;
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
        /*���Լ�����ID������(��ʾ�Լ�����Ϣ)*/
        auto pmsg = CreateIDNameLogin();
        ZinxKernel::Zinx_SendOut(*pmsg,*Ro_proto);

        /*���Լ�������Χ��ҵ�λ��*/
        pmsg = CreateRoundPlayer();
        ZinxKernel::Zinx_SendOut(*pmsg, *Ro_proto);

        /*����Χ��ҷ����Լ���λ��*/
        auto srd_list = world.GetRoundPlayer(this);
        for (auto singlePlayer : srd_list)
        {
            pmsg = CreateSelfInfo();
            auto cur_player = dynamic_cast<GameRole*>(singlePlayer);
            ZinxKernel::Zinx_SendOut(*pmsg, *(cur_player->Ro_proto));
        }

    }


    return returnValue;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(MultiMsg, recvMsg, _poUserData);
    for (auto single : recvMsg.m_Msg)
    {
        if (single->enMsgType == GameMsg::MSG_TYPE_CHAT_CONTENT)
        {
            std::cout << "type is" << single->enMsgType << std::endl;
            std::cout << single->pMsg->Utf8DebugString() << std::endl;

            std::string content = dynamic_cast<pb::Talk*>(single->pMsg)->content();
            auto role_List = ZinxKernel::Zinx_GetAllRole();
            for (auto singleRole: role_List)
            {
                auto chatMsg = CreateTalkBroadCast(content);
                auto curRole = dynamic_cast<GameRole*>(singleRole);
                ZinxKernel::Zinx_SendOut(*chatMsg, *(curRole->Ro_proto));
            }
           
        }
    }
    

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
    auto RoundP_List = world.GetRoundPlayer(this);
    for (auto singlePlayer : RoundP_List)
    {
        auto pMsg =CreateIDNameLogoff();
        auto curPlayer = dynamic_cast<GameRole*>(singlePlayer);
        ZinxKernel::Zinx_SendOut(*pMsg, *(curPlayer->Ro_proto));
    }
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
