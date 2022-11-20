#include "GameMsg.h"
#include"msg.pb.h"

GameMsg::GameMsg(MSG_TYPE _type, google::protobuf::Message* _pMsg)
	:enMsgType(_type), pMsg(_pMsg)//�������б������ԣ�������
{
	//�����л���
}
//�����л�
GameMsg::GameMsg(MSG_TYPE _type, std::string  _stream) 
	:enMsgType(_type)//�������б�  һ��
{
	//���ݲ�ͬ����Ϣ���ͣ�����ָ��ָ��ͬ������ָ��
	switch (_type)
	{
	case GameMsg::MSG_TYPE_LOGIN_ID_NAME:
		pMsg = new pb::SyncPid();
		break;
	case GameMsg::MSG_TYPE_CHAT_CONTENT:
		pMsg = new pb::Talk();
		break;
	case GameMsg::MSG_TYPE_NEW_POSTION:
		pMsg = new pb::Position();
		break;
	case GameMsg::MSG_TYPE_BROADCAST:
		pMsg = new pb::BroadCast();
		break;
	case GameMsg::MSG_TYPE_LOGOFF_ID_NAME:
		pMsg = new pb::SyncPid();
		break;
	case GameMsg::MSG_TYPE_SRD_POSTION:
		pMsg = new pb::SyncPlayers();
		break;
	default:
		break;
	}
	pMsg->ParseFromString(_stream);
}
//���л�
std::string GameMsg::serialize()
{
	std::string Output;
	pMsg->SerializeToString(&Output);
	return Output;
}
GameMsg::~GameMsg()
{
	if (NULL != pMsg)
	{
		delete pMsg;
	}
}

MultiMsg::~MultiMsg()
{
	for (auto p : m_Msg)
	{
		delete p;
	}
}
