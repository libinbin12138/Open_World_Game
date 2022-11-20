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

public:
    GameRole();
    virtual ~GameRole();

    GameProto* Ro_proto = NULL;

    // ͨ�� Irole �̳�
    virtual bool Init() override;
    virtual UserData* ProcMsg(UserData& _poUserData) override;
    virtual void Fini() override;

    // ͨ�� Player �̳�
    virtual int getX() override;
    virtual int getY() override;
};
