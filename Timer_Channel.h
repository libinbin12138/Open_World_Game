#pragma once
#include "zinx.h"
#include<sys/timerfd.h>
#include"TimeOutPrc.h"
class MyPrint_handdle :public AZinxHandler
{
    // 通过 AZinxHandler 继承
    virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
    virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;
};


class Timer_Channel : public Ichannel
{
    int time_fd;
    AZinxHandler* myprint;
public:
    // 通过 Ichannel 继承
    virtual bool Init() override;
    virtual bool ReadFd(std::string& _input) override;
    virtual bool WriteFd(std::string& _output) override;
    virtual void Fini() override;
    virtual int GetFd() override;
    virtual std::string GetChannelInfo() override;
    virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

