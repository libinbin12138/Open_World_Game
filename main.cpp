#include<iostream>
#include"GameChannel.h"
#include"GameProto.h"
#include"GameMsg.h"
#include"msg.pb.h"
#include"AOIWorld.h"
#include<random>
#include"./Timer/Timer_Channel.h"
#include"RandomName.h"
using namespace std;

extern RandomName randname;
static std::default_random_engine random_engine(time(NULL));
class myPlayer :public Player
{
	int x;
	int y;
	string name;
public:
	myPlayer(int x, int y, string name):x(x),y(y),name(name)
	{

	}
	// 通过 Player 继承
	virtual int getX() override
	{
		return x;
	}
	virtual int getY() override
	{
		return y;
	}
	string getname()
	{
		return name;
	}
};
void test_Protobuf()
{
	pb::SyncPid* pMsg = new pb::SyncPid();
	pMsg->set_pid(1);
	pMsg->set_username("test");
	GameMsg* p1 = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pMsg);
	string serailizeValue = p1->serialize();
	for (auto byte : serailizeValue)
	{
		printf("%02x ", byte);
	}
	puts("");
	cout << "序列化后的数据" << serailizeValue << endl;

	//char buff[] = { 0x08,0x01,0x12,0x04,0x74,0x65,0x73,0x74 };
	//string Value(buff, sizeof(buff));

	GameMsg* p2 = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, serailizeValue);
	cout << "反序列化后pid==" << dynamic_cast<pb::SyncPid*>(p2->pMsg)->pid() << endl;
	cout << "反序列化后username==" << dynamic_cast<pb::SyncPid*>(p2->pMsg)->username() << endl;
}
void test_Player()
{
	AOIWorld world(20, 200, 50, 230, 6, 6);
	myPlayer p1(60, 107, "玩家1");
	myPlayer p2(91, 118, "玩家2");
	myPlayer p3(147, 133, "3");
	world.AddPlayerS(&p1);
	world.AddPlayerS(&p2);
	world.AddPlayerS(&p3);

	for (auto p : world.GetRoundPlayer(&p1))
	{
		cout << "p1周围的玩家有" << dynamic_cast<myPlayer*>(p)->getname() << endl;
	}

}

void daemonlize()
{
	//1 fork
	pid_t pid = fork();
	if (pid < 0)
	{
		exit(-1);
	}
    //2 父进程退出
	if (pid > 0)
	{
		exit(0);
	}
	//3 子进程 设置回话ID
	else if (pid == 0)
	{
		setsid();
	}
	//4 子进程 设置执行路径
	int Null_fd = open("/dev/null",O_RDWR);
	//5 子进程 重定向3个文件描述到/dev/null
	if (Null_fd >= 0)
	{
		dup2(Null_fd, 0);
		dup2(Null_fd, 1);
		dup2(Null_fd, 2);
		close(Null_fd);
	}	
	//6.设置进程监控
	while (1)
	{
		pid_t pid = fork();
		if (0 > pid)
		{
			exit(-1);
		}
		else if (pid > 0)
		{
			int status = 0;
			wait(&status);
			if (status == 0)
			{
				exit(0);
			}
		}
		else 
		{
			break;

		}
	}
}
int main()
{
	//test_Player();
	//test_Protobuf();
	daemonlize();
	randname.LodaFile();
	ZinxKernel::ZinxKernelInit();
	ZinxKernel::Zinx_Add_Channel(*(new ZinxTCPListen(8989,new Game_ChanFact())));
	ZinxKernel::Zinx_Add_Channel(*(new Timer_Channel()));
	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();

	return 0;
}