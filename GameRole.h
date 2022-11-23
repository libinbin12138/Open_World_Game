#pragma once
#include<zinx.h>
#include"AOIWorld.h"
#include"GameMsg.h"

class GameProto;

class GameRole :
    public Irole,public Player
{
    float x=0;
    float z=0;
    float y=0;//高
    float v=0;//角度
    int Pid;
    std::string Player_Name;

    GameMsg* CreateIDNameLogin();
    GameMsg* CreateRoundPlayer();
    GameMsg* CreateSelfInfo();
    GameMsg* CreateIDNameLogoff();

    GameMsg* CreateTalkBroadCast(std::string _content);//聊天
public:
    GameRole();
    virtual ~GameRole();

    GameProto* Ro_proto = NULL;

    void ProcChat(std::string _content);
    void ProcMovement(float _x, float _y, float _z, float _v);
    // 通过 Irole 继承
    virtual bool Init() override;
    virtual UserData* ProcMsg(UserData& _poUserData) override;
    virtual void Fini() override;

    // 通过 Player 继承
    virtual int getX() override;
    virtual int getY() override;
};

