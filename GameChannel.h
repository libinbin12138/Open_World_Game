#pragma once
#include<ZinxTCP.h>
#include"GameProto.h"
class GameChannel :
    public ZinxTcpData
{
    
public:
    // 通过 ZinxTcpData 继承
    GameChannel(int fd);
    virtual ~GameChannel();
    GameProto* ch_proto = NULL;

    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};


class Game_ChanFact :public IZinxTcpConnFact
{
    // 通过 IZinxTcpConnFact 继承
    virtual ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};