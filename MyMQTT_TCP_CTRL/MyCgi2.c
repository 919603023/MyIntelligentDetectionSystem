#include <stdio.h> /* FILE */
#include <fcntl.h>/* O_RDWR */
#include <string.h>/* memset() */
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>/* memset() */
#include <sys/ipc.h>
#include <sys/msg.h>
#include <semaphore.h>
#include "sqlite3.h"
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <mosquitto.h>
#include <time.h>
#include <pthread.h>
#define MQTTTOCGI 10
#define CGITOMQTT 20
#define FLUSH 0
#define FLUSHBACK 1
typedef struct{
    long type;
    int My_type;
    char data[512];
}MSG;
void *My_Time(void *arg)
{
    sleep(10);
    printf("失去连接");
    exit(1);
}
int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_create(&tid,NULL,My_Time,NULL);
    pthread_detach(tid);
	printf("content-type:text/html\n\n");
	char *len = getenv("CONTENT_LENGTH");
        // 再使用fgets的实际的数据
        char data[128]="";
        fgets(data,atoi(len)+1,stdin);

    key_t key = ftok("My_server",2020);
   int msg_id = msgget(key,IPC_CREAT | 0666);
    if(msg_id < 0)
    {
        printf("error");
        return 0;
    }
	if(strncmp("refresh",data,7) == 0)
	{
        
        MSG msg;
        msg.type = CGITOMQTT;
        msg.My_type = FLUSH;
		msgsnd(msg_id,&msg,sizeof(msg)-sizeof(long),0);
        msgrcv(msg_id,&msg,sizeof(msg)-sizeof(long),MQTTTOCGI,0);
		if(msg.My_type == FLUSHBACK)
        {
            printf("%s",msg.data);
        }
	}
	else if(strncmp("Changepassword:",data,15) == 0)
	{
		char user[50] = "",oldpasswd[50] = "",newpasswd[50] = "";
		sscanf(data,"%*[^:]:%[^,],%[^,],%s",user,oldpasswd,newpasswd);
		char sql[100]="";
		
		sprintf(sql,"select pwd from test where id=\"%s\";",user);
		printf("%s\n",sql);
		
	}
	
return 0;
}
