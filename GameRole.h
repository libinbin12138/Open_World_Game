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
    float y=0;//��
    float v=0;//�Ƕ�
    int Pid;
    std::string Player_Name;

    GameMsg* CreateIDNameLogin();
    GameMsg* CreateRoundPlayer();
    GameMsg* CreateSelfInfo();
    GameMsg* CreateIDNameLogoff();

    GameMsg* CreateTalkBroadCast(std::string _content);//����
public:
    GameRole();
    virtual ~GameRole();

    GameProto* Ro_proto = NULL;

    void ProcChat(std::string _content);
    void ProcMovement(float _x, float _y, float _z, float _v);
    // ͨ�� Irole �̳�
    virtual bool Init() override;
    virtual UserData* ProcMsg(UserData& _poUserData) override;
    virtual void Fini() override;

    // ͨ�� Player �̳�
    virtual int getX() override;
    virtual int getY() override;
};

