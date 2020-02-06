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
    write(fd,"POWEROFF",sizeof("POWEROFF"));
    char buf[10];
	
    
}