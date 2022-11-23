#include "GameRole.h"
#include"GameMsg.h"
#include"GameProto.h"
#include"msg.pb.h"
#include"GameChannel.h"
#include <iostream>
#include<algorithm>
#include<iostream>
#include<random>
#include"./Timer/TimeOutPrc.h"
class ExitTask :public TimeOutPrc//定时退出
{
    // 通过 TimeOutPrc 继承
    virtual void prc() override
    {

        ZinxKernel::Zinx_Exit();
    }
    virtual int gettimer() override
    {
        return 20;
    }
};
static ExitTask exitTask;//定时退出

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
    pSelf->set_tp(2);//和客户端对应，2就是发送自己的消息给自己
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
    Chatinfo->set_tp(1);//1就是聊天
    Chatinfo->set_content(_content);

    GameMsg* ChatMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, Chatinfo);
    return ChatMsg;
}

static std::default_random_engine random_engine(time(NULL));

GameRole::GameRole()
{
    x = 100+ random_engine() % 50;
    z = 100 + random_engine() % 50;
    Player_Name ="Tom";
}

GameRole::~GameRole()
{
}

void GameRole::ProcChat(std::string _content)
{
        /*std::cout << "接收到的消息id是" << single->enMsgType << std::endl;
        std::cout << single->pMsg->Utf8DebugString() << std::endl;*/

        //std::string content = dynamic_cast<pb::Talk*>(single->pMsg)->content();
        auto role_List = ZinxKernel::Zinx_GetAllRole();

        for (auto singleRole : role_List)
        {
            auto chatMsg = CreateTalkBroadCast(_content);//不懂这里为啥循环里外不一样。。。
           // printf("1address of pointer is: 0x%0X\n", &chatMsg);

            auto curRole = dynamic_cast<GameRole*>(singleRole);
            ZinxKernel::Zinx_SendOut(*chatMsg, *(curRole->Ro_proto));
           // printf("3address of pointer is: 0x%0X\n", &chatMsg);
        }
}

void GameRole::ProcMovement(float _x, float _y, float _z, float _v)
{      
    auto Plist_old = world.GetRoundPlayer(this);
    world.DelPlayers(this);

    x = _x;
    y = _y;
    z = _z;
    v = _v;
    world.AddPlayerS(this);
    auto Plist_New = world.GetRoundPlayer(this);

    //遍历新的，不属于旧的则视野出现
    for (auto single_new_player : Plist_New)
    {
        if (Plist_old.end() == find(Plist_old.begin(), Plist_old.end(), single_new_player))
            //遍历到旧视野的末，是新出现的
        {
            ViewAppear(dynamic_cast<GameRole*>(single_new_player));
        }
    }
    for (auto single_old_player : Plist_old)
    {
        if (Plist_New.end() == find(Plist_New.begin(), Plist_New.end(), single_old_player))
            //遍历到新视野的末，是旧人物
        {
            ViewLost(dynamic_cast<GameRole*>(single_old_player));
        }
    }


        auto role_List = world.GetRoundPlayer(this);
        for (auto singleRole : role_List)
        {
            pb::BroadCast* NewPosition = new pb::BroadCast();
            NewPosition->set_pid(Pid);
            NewPosition->set_username(Player_Name);
            NewPosition->set_tp(4);
            auto pPosition = NewPosition->mutable_p();;
            pPosition->set_x(_x);
            pPosition->set_y(_y);
            pPosition->set_z(_z);
            pPosition->set_v(_v);
            auto curRole = dynamic_cast<GameRole*>(singleRole);
            ZinxKernel::Zinx_SendOut(*(new GameMsg(GameMsg::MSG_TYPE_BROADCAST, NewPosition)), *(curRole->Ro_proto));
        }
   
}

void GameRole::ViewAppear(GameRole* player)
{
    //向自己发送新玩家的信息
    auto  pMsg = player->CreateSelfInfo();
    ZinxKernel::Zinx_SendOut(*pMsg, *Ro_proto);

    //向新玩家发送自己的信息
    pMsg =CreateSelfInfo();
    ZinxKernel::Zinx_SendOut(*pMsg,*( player->Ro_proto));

  
}

void GameRole::ViewLost(GameRole* player)
{
    //向自己发送旧玩家下线的信息
    auto pMsg = player->CreateIDNameLogoff();
    ZinxKernel::Zinx_SendOut(*pMsg, *Ro_proto);

    //向旧玩家发送自己下线的信息
    pMsg = CreateIDNameLogoff();
    ZinxKernel::Zinx_SendOut(*pMsg, *(player->Ro_proto));
}

bool GameRole::Init()
{
    if (ZinxKernel::Zinx_GetAllRole().size() <= 0)
    {
        TimeOutMsg::getInstance().delTimeOutPrc(&exitTask);
    }
    /*设置玩家ID为当前连接的fd*/
    //由getfd来实现id的唯一性
    //不能在构造函数中初始话，因为那里还没有连接Ro_proto        
    Pid = this->Ro_proto->m_channel->GetFd();

    bool returnValue = false;

    //把自己这个role添加到world中
    returnValue = world.AddPlayerS(this);

    if (returnValue == true)
    {
        /*向自己发送ID和名称(显示自己的信息)*/
        auto pmsg = CreateIDNameLogin();
        ZinxKernel::Zinx_SendOut(*pmsg,*Ro_proto);

        /*向自己发送周围玩家的位置*/
        pmsg = CreateRoundPlayer();
        ZinxKernel::Zinx_SendOut(*pmsg, *Ro_proto);

        /*向周围玩家发送自己的位置*/
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
        auto NewPos = dynamic_cast<pb::Position*>(single->pMsg);
        switch (single->enMsgType)
        {
        case GameMsg::MSG_TYPE_CHAT_CONTENT:
            ProcChat(dynamic_cast<pb::Talk*>(single->pMsg)->content());
        break;       
        case GameMsg::MSG_TYPE_NEW_POSTION:
            ProcMovement(NewPos->x(), NewPos->y(), NewPos->z(), NewPos->v());
        break;
        default:
            break;
       }
    }

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
    auto RoundP_List = world.GetRoundPlayer(this);
    for (auto singlePlayer : RoundP_List)
    {
        auto pMsg =CreateIDNameLogoff();
        auto curPlayer = dynamic_cast<GameRole*>(singlePlayer);
        ZinxKernel::Zinx_SendOut(*pMsg, *(curPlayer->Ro_proto));
    }
    world.DelPlayers(this);

    if (ZinxKernel::Zinx_GetAllRole().size() <=1)
    {
        TimeOutMsg::getInstance().addTimeOutPrc(&exitTask);
    }

}

int GameRole::getX()
{
    return (int)x;
}

int GameRole::getY()
{
    return (int)z;
}
