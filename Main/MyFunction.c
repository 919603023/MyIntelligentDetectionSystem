#include "MyFunction.h"
int msg_id;
int My_flag = 0;
struct mosquitto *m_hMqtt;
/************MQTT接受线程函数************************/
void *MyMQTT_INIT(void *arg)
{

	//MQTT的数据接收和发送

	MQTT_Init(m_hMqtt);

	MQTT_Destroy(m_hMqtt);
}

int MQTT_Init(struct mosquitto *m_hMqtt)
{
	printf("MQTT_INIT");
	char *topic1 = MQTTTOSERVER;
	char *topic2 = "humidity";

	//初始化lib库函数
	mosquitto_lib_init();
	m_hMqtt = mosquitto_new("test", true, "data");

	//设置连接确认回调
	mosquitto_connect_callback_set(m_hMqtt, my_connect_callback);
	//设置断开连接确认回调
	mosquitto_disconnect_callback_set(m_hMqtt, my_disconnect_callback);

	//设置订阅回调(mosquitto_subscribe()订阅后回调)
	mosquitto_subscribe_callback_set(m_hMqtt, my_subscribe_callback);

	//设置收到订阅回调
	mosquitto_message_callback_set(m_hMqtt, my_message_callback);

	//开始连接服务器
	if (MOSQ_ERR_SUCCESS == mosquitto_connect(m_hMqtt, MQTTIP, MQTTPORT, 60))
	{
		puts("连接服务器成功");
	}
	else
	{
		puts("连接服务器失败");
		return -1;
	}

	mosquitto_subscribe(m_hMqtt, NULL, MQTTTOSERVER, 1);
	mosquitto_loop_start(m_hMqtt);
	/* 阻塞等待 */
	mosquitto_loop_stop(m_hMqtt, false);
	mosquitto_destroy(m_hMqtt);
	mosquitto_lib_cleanup();
	return 0;
}
int MQTT_Destroy(struct mosquitto *m_hMqtt)
{
	mosquitto_destroy(m_hMqtt);
	mosquitto_lib_cleanup();
}

// 连接回调函数，当连接成功时会进入这里，可以在这里进行连接成功之后的操作，比如连接之后的信息同步
void my_connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
}
// 断开连接回调函数，在断开连接之后进入
void my_disconnect_callback(struct mosquitto *mosq, void *obj, int result)
{
	printf("%d:\n", __LINE__);
}

// 订阅消息回调
void my_subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
	int i;
	time_t t;
	struct tm *lt;
	time(&t);
	lt = localtime(&t);
	printf("%d: %s\n", __LINE__, (char *)obj);
	printf("%d: mid=%d\n", __LINE__, mid);
	printf("%d: qos_count=%d\n", __LINE__, qos_count);
	for (i = 0; i < qos_count; i++)
		printf("%d: granted_qos[%d]=%d\n", __LINE__, i, granted_qos[i]);
}

/***********************收到消息的回调函数**********************/
// 消息回调
void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	if (My_flag == 0)
	{
		My_flag = 1;
		return;
	}
	time_t t;
	struct tm *lt;
	time(&t);
	char databuf[128] = "";
	char timebuf[100] = "";
	lt = localtime(&t);
	printf("%d.%d.%d %d:%d:%d ", lt->tm_year + 1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
	printf("%d:topic(%s)->%s\n", __LINE__, (char *)msg->topic, (char *)msg->payload);
	sprintf(timebuf,"%d.%d.%d %d:%d:%d ", lt->tm_year + 1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
	if (strcmp((char *)msg->topic, MQTTTOSERVER) == 0) //收到mqtt给服务器的消息后
													   //使用消息队列传送给对应进程
	{
		int flag = 0;
		printf("收到mqtt给服务器的消息");
		MSG My_msg;
		//mosquitto_publish(mosq,NULL,SERVERTOMQTT,strlen((char *)msg->payload),(char *)msg->payload,1,true);
		My_msg.type = MQTTTOCGI;
		if (strcmp((char *)msg->payload, "OK") == 0)
		{
			My_msg.My_type = OK;
			flag = 1;
		}
		else if (strncmp((char *)msg->payload, "DataBack:", 9) == 0)
		{
			int  *nRow, *nColumn;
			My_msg.My_type = FLUSHBACK;
			flag = 1;
			
			sscanf((char *)msg->payload,"%*[^:]:%s",databuf);
			char buf[128] = "";
			sprintf(buf,"('%s','%s')",timebuf,databuf);
			char * errmsg = NULL;
			char **dbResult;
			MySqlite3(DBNAME,NULL,HistoricalRecordTable,buf,nRow,nColumn,errmsg,&dbResult,0);
			
		}

		if (flag == 1)
		{
			strcpy(My_msg.data, (char *)msg->payload);
			msgsnd(msg_id, &My_msg, sizeof(My_msg) - sizeof(long), 0);
		}
	}
}

/************************************MQTT接受线程函数结束********************/

void My_Kill(int sig) //ctrl+c退出，销毁消息队列，使网页无法操作消息队列
{
	printf("安全结束\n");
	msgctl(msg_id, IPC_RMID, NULL);
	kill(0, 9);
}

/******************************************************/
int publish_subscribe(char *topic, char *data) //发布订阅
{

	struct mosquitto *mhMqtt;

	//初始化lib库函数
	mosquitto_lib_init();
	// 定义一个客户端名为subdemo150的发布端。客户端表示订阅端的唯一性
	mhMqtt = mosquitto_new("test1", true, "data");

	//设置连接确认回调
	mosquitto_connect_callback_set(mhMqtt, my_connect_callback);
	//设置断开连接确认回调
	mosquitto_disconnect_callback_set(mhMqtt, my_disconnect_callback);

	//mosquitto_loop_start(m_hMqtt);

	//开始连接服务器
	mosquitto_connect(mhMqtt, MQTTIP, 1883, 1);

	mosquitto_publish(mhMqtt, NULL, topic, strlen(data), data, 1, true); //发布数据

	/* 阻塞等待 */
	//mosquitto_loop_stop(m_hMqtt, false);
	mosquitto_destroy(mhMqtt);
	mosquitto_lib_cleanup();
	return 0;
}
/***************************获取cgl信息，发布mqtt**************/
void *CGI_CMD_GET_TO_MQTT(void *arg)
{
	
	MSG My_msg;

	while (1)
	{

		msgrcv(msg_id, &My_msg, sizeof(My_msg) - sizeof(long), CGITOMQTT, 0);
		if (My_msg.My_type == FLUSH)
		{
			printf("收到FLUSH\n");
			publish_subscribe(SERVERTOMQTT, "FLUSH");
		}
		else if (My_msg.My_type == _1_LEDCTRL)
		{
			printf("收到1_LED的控制\n");

			publish_subscribe(SERVERTOMQTT, My_msg.data);
		}
	}
}
/***********************获取cgl信息，操作数据库**************/
void *CGI_CMD_GET_TO_SQLITE3(void *arg)
{
	
	MSG My_msg;

	while (1)
	{

		msgrcv(msg_id, &My_msg, sizeof(My_msg) - sizeof(long), CGITOSQLITE3, 0);
		if (My_msg.My_type == GETHistoricalRecord)
		{
			printf("收到获取历史信息\n");
			GET_HistoricalRecord();
		}
		else if (My_msg.My_type == _1_LEDCTRL)
		{
			printf("收到1_LED的控制\n");

			publish_subscribe(SERVERTOMQTT, My_msg.data);
		}
	}
}
/**************************************************/
/***********************sqlite3功能函数***********
 * 可实现查询，插入等功能
 * 参数：
 * 		char *dbname:数据库名
 * 		char *column：结果返回的列	
 * 		char *table：在哪个表
 * 		char *condition：条件
 * 		int type 类型
 * 						0  //插入
 * 						1  //查询
 * 返回值：	
 * 		0，为成功
 * 		-1，为数据库打开失败
 * 		-2,为类型有错
 ************************************************/
int MySqlite3(char *dbname,char *column,char *table,char *condition,int *nRow,int *nColumn,char * errmsg,char ***dbResult,int type)
{
    sqlite3 *db = NULL;
	char sql[512] = "";
   int ret = sqlite3_open(dbname, &db);
    if (ret != SQLITE_OK)
	{
		printf("数据库打开失败\n");
		return -1;
	}
    if(type == 0)//插入
	{
		sprintf(sql,"insert into %s values %s;",table,condition);
	}
	else if(type == 1)//查询
	{
		sprintf(sql,"select %s from %s ;",column,table);
	}
	else 
	{
		return -2;
	}
	int tmp = sqlite3_get_table(db, sql,dbResult, nRow, nColumn, &errmsg);
	

	

	sqlite3_close(db);
 
 return tmp;
}

void GET_HistoricalRecord()
{
	int result, nRow, nColumn;
	char * errmsg = NULL;
	char **dbResult;
	
	
	char *condition = "";
	if(MySqlite3(DBNAME,"*",HistoricalRecordTable,condition,&nRow,&nColumn,errmsg,&dbResult,1) < 0);
	{
		printf("查询出错\n");
	}
	
		int i, j, index;
		int flag = 0;
		MSG My_msg;
		My_msg.type = SQLITE3TOCGI;
		My_msg.My_type = BACKHistoricalRecord;
		index = nColumn;
		if(nRow > 0)
		{
			for(int i = 0 ; i < nColumn ;i++)
			{
			    sprintf(My_msg.data,"-%s",dbResult[i]);
				msgsnd(msg_id, &My_msg, sizeof(My_msg) - sizeof(long), 0);
			}
			
		}
		for(i=0;i<nRow;i++)
		{

				printf("nColumn[%d]=%s:data[%d]=%s\n",j,dbResult[j],index,dbResult[index]);
				
				sprintf(My_msg.data,"*%s",dbResult[index]);
				index++;
				msgsnd(msg_id, &My_msg, sizeof(My_msg) - sizeof(long), 0);
				bzero(My_msg.data,sizeof(My_msg.data));
			
		}
		My_msg.My_type = BACKOVERHistoricalRecord;
		msgsnd(msg_id, &My_msg, sizeof(My_msg) - sizeof(long), 0);
		sqlite3_free_table(dbResult);
	

}