#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <mosquitto.h>
#include <time.h>

// 连接回调函数，当连接成功时会进入这里，可以在这里进行连接成功之后的操作，比如连接之后的信息同步
void my_connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
}
// 断开连接回调函数，在断开连接之后进入
void my_disconnect_callback(struct mosquitto *mosq, void *obj, int result)
{
	printf("%d:\n",__LINE__);
}
// 消息回调
void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	time_t t;
	struct tm *lt;
	time(&t);
	lt =  localtime(&t);
	printf("%d.%d.%d %d:%d:%d ",lt->tm_year+1900,lt->tm_mon,lt->tm_mday,lt->tm_hour,lt->tm_min,lt->tm_sec);
	printf("%d:topic(%s)->%s\n",__LINE__,(char *)msg->topic,(char *)msg->payload);	
}
// 订阅消息回调
void my_subscribe_callback(struct mosquitto *mosq, void *obj, int mid,int qos_count,const int *granted_qos)
{
	int i;
	time_t t;
	struct tm *lt;
	time(&t);
	lt =  localtime(&t);
	printf("%d: %s\n",__LINE__,(char *)obj);
	printf("%d: mid=%d\n",__LINE__,mid);
	printf("%d: qos_count=%d\n",__LINE__,qos_count);
	for(i=0;i<qos_count;i++)
		printf("%d: granted_qos[%d]=%d\n",__LINE__,i,granted_qos[i]);
}
int main(int argc,char *argv[])
{
	struct mosquitto *m_hMqtt;
	char *topic1 = "tmp";
	char *topic2 = "humidity";
	char *content = "hello";
	
	//初始化lib库函数
	mosquitto_lib_init();
    m_hMqtt = mosquitto_new("pubtest", true, "data");
	
	//设置连接确认回调
	mosquitto_connect_callback_set(m_hMqtt, my_connect_callback);
 	//设置断开连接确认回调
    mosquitto_disconnect_callback_set(m_hMqtt, my_disconnect_callback);
	
	//设置订阅回调(mosquitto_subscribe()订阅后回调)
	mosquitto_subscribe_callback_set(m_hMqtt, my_subscribe_callback);
	
	//设置收到订阅回调
	mosquitto_message_callback_set(m_hMqtt, my_message_callback);
	
	//开始连接服务器
	if(MOSQ_ERR_SUCCESS == mosquitto_connect(m_hMqtt, "129.211.89.48", 1883, 60))
		puts("连接服务器成功");
	else
		puts("连接服务器失败");
	mosquitto_subscribe(m_hMqtt,NULL,topic1,1);
	//mosquitto_subscribe(m_hMqtt,NULL,topic2,1);
	mosquitto_publish(m_hMqtt,NULL,topic1,strlen(content),content,1,true);
	mosquitto_loop_start(m_hMqtt);
	/* 阻塞等待 */
	mosquitto_loop_stop(m_hMqtt, false);
	
	mosquitto_destroy(m_hMqtt);
    mosquitto_lib_cleanup();
	return 0;
}