#include "GameProto.h"
#include"GameChannel.h"
#include"GameMsg.h"
#include"GameRole.h"

using namespace std;

GameProto::GameProto()
{
}

GameProto::~GameProto()
{
    if (NULL != Pr_role)
    {
        ZinxKernel::Zinx_Del_Role(*Pr_role);
        delete Pr_role;
    }
}

UserData* GameProto::raw2request(std::string _szInput)
{
    MultiMsg* ParseMsg = new MultiMsg();
    szLast.append(_szInput);

    while(1)
    {
        if (szLast.size() < 8)
        {
            break;
        }
        /*在前四个字节中读取消息内容长度*/
        int iLength = 0;//左移
        iLength |= szLast[0]<<0;
        iLength |= szLast[1] << 8;
        iLength |= szLast[2] << 16;
        iLength |= szLast[3] << 24;

        int id = 0;
        id |= szLast[4] << 0;
        id |= szLast[5] << 8;
        id |= szLast[6] << 16;
        id |= szLast[7] << 24;

        if (szLast.size() - 8 < iLength)
        {
            /*本条报文还没够，啥都不干*/
            break;
        }

        GameMsg* pmsg = new GameMsg((GameMsg::MSG_TYPE)id, szLast.substr(8,iLength));
        ParseMsg->m_Msg.push_back(pmsg);
        szLast.erase(0, 8 + iLength);
    }
    for (auto single : ParseMsg->m_Msg)
    {
        cout << single->pMsg->Utf8DebugString() << endl;
        //按序列化格式打印
    }
   /* pb::Talk* pmsg = new pb::Talk();
    pmsg->set_content("hello");
    GameMsg* pGameMsg = new GameMsg(GameMsg::MSG_TYPE_CHAT_CONTENT, pmsg);
    ZinxKernel::Zinx_SendOut(*(pGameMsg), *this);*/

    return ParseMsg;
}

std::string* GameProto::response2raw(UserData& _oUserData)
{
    int length = 0;
    int id = 0;
    string MsgContemnt;

    GET_REF2DATA(GameMsg, sendData, _oUserData);
    MsgContemnt = sendData.serialize();
    length = MsgContemnt.size();
    id = sendData.enMsgType;

    auto pret = new string();
    pret->push_back((length >> 0) & 0xff);
    pret->push_back((length >> 8) & 0xff);
    pret->push_back((length >> 16) & 0xff);
    pret->push_back((length >> 24) & 0xff);
    pret->push_back((id >> 0) & 0xff);
    pret->push_back((id >> 8) & 0xff);
    pret->push_back((id >> 16) & 0xff);
    pret->push_back((id >> 24) & 0xff);
    pret->append(MsgContemnt);

    return pret;
}

Irole* GameProto::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
    return Pr_role;
}

Ichannel* GameProto::GetMsgSender(BytesMsg& _oBytes)
{
    return m_channel;
}
