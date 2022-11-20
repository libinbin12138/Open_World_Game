#pragma once
#include<ZinxTCP.h>
#include"GameProto.h"
class GameChannel :
    public ZinxTcpData
{
    
public:
    // ͨ�� ZinxTcpData �̳�
    GameChannel(int fd);
    virtual ~GameChannel();
    GameProto* ch_proto = NULL;

    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};


class Game_ChanFact :public IZinxTcpConnFact
{
    // ͨ�� IZinxTcpConnFact �̳�
    virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};