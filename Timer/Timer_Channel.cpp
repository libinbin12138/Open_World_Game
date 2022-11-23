#include "Timer_Channel.h"



bool Timer_Channel::Init()
{
   myprint = new MyPrint_handdle();

    int fd = timerfd_create(CLOCK_MONOTONIC,0);
    bool status = false;
    if (fd >= 0)
    {
        this->time_fd = fd;
    }
    struct itimerspec period = {
        {1,0},{1,0}
    };
    int ret=timerfd_settime(time_fd, 0, &period, NULL);
    if (ret == 0)
    {
        status = true;
    }
    return status;
}

bool Timer_Channel::ReadFd(std::string& _input)
{
    char buf[8] = { 0 };
    int len=read(time_fd, buf, sizeof(buf));
    bool status = false;
    if (len == sizeof(buf))
    {
        status = true;
    }
    _input.assign(buf, sizeof(buf));
    return status;
}

bool Timer_Channel::WriteFd(std::string& _output)
{
    return false;
}

void Timer_Channel::Fini()
{
    close(time_fd);
    time_fd = -1;
    if (myprint != NULL)
    {
        delete myprint;
        myprint = NULL;
    }
}

int Timer_Channel::GetFd()
{
    return this->time_fd;
}

std::string Timer_Channel::GetChannelInfo()
{
    return "timeInfo";
}

AZinxHandler* Timer_Channel::GetInputNextStage(BytesMsg& _oInput)
{
   // return this->myprint;
    return &TimeOutMsg::getInstance();
}



IZinxMsg* MyPrint_handdle::InternelHandle(IZinxMsg& _oInput)
{
   auto outchannel=ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
    std::string value = "hello world";
    ZinxKernel::Zinx_SendOut(value, *outchannel);
    return nullptr;
}

AZinxHandler* MyPrint_handdle::GetNextHandler(IZinxMsg& _oNextMsg)
{
    return nullptr;
}
