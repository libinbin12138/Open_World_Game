#include "GameChannel.h"
#include"GameRole.h"

GameChannel::GameChannel(int fd) 
	:ZinxTcpData(fd)
{

}
GameChannel::~GameChannel()
{
	
	if (NULL != ch_proto)
	{
		ZinxKernel::Zinx_Del_Proto(*ch_proto);
		delete ch_proto;
	}
}

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return ch_proto;
}


ZinxTcpData* Game_ChanFact::CreateTcpDataChannel(int _fd)
{
	/*����tcpͨ������*/
	auto game_chaneel = new GameChannel(_fd);
	/*����Э�����*/
	auto proto = new GameProto();
	/*����GameRole����*/
	auto game_Role = new GameRole();

	/*��Э�����*/
	game_chaneel->ch_proto = proto;
	/*Э������ͨ������*/
	proto->m_channel = game_chaneel;

	//��ɫ������Э������
	proto->Pr_role = game_Role;
	game_Role->Ro_proto = proto;
	

	ZinxKernel::Zinx_Add_Proto(*proto);
	ZinxKernel::Zinx_Add_Role(*game_Role);

	return game_chaneel;
}
