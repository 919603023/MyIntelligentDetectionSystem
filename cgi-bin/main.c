#include "fun.h"

int main()
{
	
	ReadConfig_Filter_MsgFile();
	getinterface(); // 获取自身网卡信息
// WriteEth_MsgFile();
	ReadConfig_Route_MsgFile();//获取路由表

	

	pthread_t tid;

	pthread_create(&tid,NULL,Communication,NULL);

	pthread_detach(tid);

	int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	
	CONFIG_FILTER_MSG *head = Filter_Msg;
	if (head != NULL)
		
	while (head != NULL)
	{
		for (int i = 0; i < 4; i++)
		{
			printf("%d.", head->Filter_Src_Ip[i]);
		}
		printf("\n");
		for (int i = 0; i < 6; i++)
		{
			printf("%02x-", head->Filter_Src_Mac[i]);
		}
		printf("\n");
		printf("%s \n",head->Filter_Name);

		if (head->next == NULL)
			break;
	}

	while (1)
	{

		unsigned char buf[1500] = "";

		int len = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);

		if (AnalyzeAgreement(buf) == -1)
		{
			continue;
		}

		if (mybuf.type == ARP_BACK)
		{
				//如果是ARP请求，使用函数查看是否有该IP的记录，
                        //如果没有该IP的记录，则使用插入节点函数，插入节点
                         //如果返回0证明列表插进去了，如果是1证明查到了ARP记录
			if (ArpDispose(mybuf.src_ip, NULL, mybuf.dst_mac, INSERT) == -1)
			{
				continue;
			}
				
		}
		//是否是同一网段
		int Ethnum;

		Ethnum = IsSameSegment();
		//有同一网段，返回所出去的网卡
		//如果所出网卡为-1，证明没有不是本路由器网段
		//需要查路由表获取目的IP和目的
		if (Ethnum == -1)
		{
			//如果获取的网卡仍然是-1，证明，路由表中没有该目的IP的下一跳
			//直接丢包
			Ethnum = Config_Route_MsgDispose(mybuf.dst_ip,buf+6, mybuf.dst_ip,NULL,NULL, FIND);
			if (Ethnum != -1)
			{
				//路由表有同一网段，返回所出去的网卡
				
			}
			else
			{
				continue;
			}
		}
		//查ARP表
		//查ARP表
                 //如果查到，将目的buf中的目的mac赋值为此IP的mac
                //如果查不到，应该发送ARP请求，并跳过本次循环
		if (ArpDispose(mybuf.dst_ip, NULL, mybuf.dst_mac, FIND) == -1)
		{
			SendArp(Ethnum,0,fd,NULL);
			continue;
		}
			memcpy(buf, mybuf.dst_mac, 6);
			memcpy(buf + 6, net_interface[Ethnum].mac, 6);
			SendTo(len, buf, Ethnum, fd);
			printf("%s\n",net_interface[Ethnum].name);
		  bzero(&mybuf, sizeof(mybuf));
	}
	close(fd);
	return 0;
}
