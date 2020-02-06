#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    //用printf输出
    printf("content-type:text/html\n\n");
    char *FileName = "../HTML/Config_Route_Msg";

    int ret = mkfifo("/home/wgb/boa/www/cgi-bin/myfifo", 0664);

	if (ret < 0)
	{
		perror("mkfifo");
	}

	int fd = open("/home/wgb/boa/www/cgi-bin/myfifo", O_RDWR);
    write(fd,"CONFIG_ROUTE_MSG",sizeof("CONFIG_ROUTE_MSG"));
	FILE *fp;
	if ((fp = fopen(FileName, "a")) == NULL)
	{
		perror("fail to fopen");
		return -1;
	}
    
    //获取服务器收到的数据 如果是GET方式 数据直接使用getenv获取
    //getenv的参数为："QUERY_STRING"
    char *data = getenv("QUERY_STRING");
//Route_IP=3245&Route_Mask=54&Route_NextHop=63

    char IP[300],MASK[300],NEXTHOP[300];
    char ch ;
   sscanf(data,"%[^+]+%[^+]+%[^+]+",IP, MASK,NEXTHOP);
    
    fputc('{',fp);
    fputc('\n',fp);
    fputs(IP,fp);
     fputc('\n',fp);
     fputs(MASK,fp);
     fputc('\n',fp);
    fputs(NEXTHOP,fp);
    fputc('\n',fp);
fputc('}',fp);
    fclose(fp);
    return 0;
}