
#include "MyFunction.h"



int main(int argc,char const *argv[])
{
    //数据库信息
    int ret;
    // sqlite3 *db;
    // ret = sqlite3_open("first.db",&db);
	// if(ret != SQLITE_OK)
	// 	printf("数据库打开失败\n");
    signal(SIGINT,My_Kill);//注册ctrl+c信号

   
    key_t key = ftok("My_server",2020);
    msg_id = msgget(key,IPC_CREAT | 0666);
    
    
    pthread_t MQTT_INIT,CGIGET;
 pthread_create(&MQTT_INIT,NULL,MyMQTT_INIT,NULL);
    
  pthread_create(&CGIGET,NULL,CGI_CMD_GET,NULL);
    while (1)

    {
        sleep(1);
        // publish_subscribe("mqtttoserver","niha"); 
        
    }
    
}
