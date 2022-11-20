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
	/*创建tcp通道对象*/
	auto game_chaneel = new GameChannel(_fd);
	/*创建协议对象*/
	auto proto = new GameProto();
	/*创建GameRole对象*/
	auto game_Role = new GameRole();

	/*绑定协议对象*/
	game_chaneel->ch_proto = proto;
	/*协议对象绑定通道对象*/
	proto->m_channel = game_chaneel;

	//角色对象与协议对象绑定
	proto->Pr_role = game_Role;
	game_Role->Ro_proto = proto;
	

	ZinxKernel::Zinx_Add_Proto(*proto);
	ZinxKernel::Zinx_Add_Role(*game_Role);

	return game_chaneel;
}
