#pragma once
#include<zinx.h>
#include<list>

class GameChannel;
class GameRole;

class GameProto :
    public Iprotocol
{
    std::string szLast;

public:
    GameChannel* m_channel = NULL;
    GameRole * Pr_role = NULL;

    GameProto();
    virtual ~GameProto();
    // Í¨¹ý Iprotocol ¼Ì³Ð
    virtual UserData* raw2request(std::string _szInput) override;
    virtual std::string* response2raw(UserData& _oUserData) override;
    virtual Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
    virtual Ichannel* GetMsgSender(BytesMsg& _oBytes) override;
};

