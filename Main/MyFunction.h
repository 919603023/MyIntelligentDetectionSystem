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
#ifndef _MYFUNCTION_H_
#define _MYFUNCTION_H_
#define DBNAME "HistoricalRecord.db"
#define HistoricalRecordTable "HistoricalRecord"
#define SERVERTOMQTT "servertomqtt"
#define MQTTTOSERVER "mqtttoserver"
#define MQTTTOCGI 10
#define CGITOMQTT 20
#define CGITOSQLITE3 30
#define SQLITE3TOCGI 40
#define BACKHistoricalRecord 0
#define BACKOVERHistoricalRecord 1
#define GETHistoricalRecord 0
#define FLUSH 0
#define FLUSHBACK 1
#define _1_LEDCTRL 2
#define _2_LEDCTRL 3
#define OK   2
// #define MQTTIP "129.211.89.48"
#define MQTTIP "192.168.2.225"
#define MQTTPORT 1883
typedef struct{
    long type;
    int My_type;
    char data[512];
}MSG;
/**********************消息队列类型**************
 * type        My_type        
 * CGITOMQTT   FLUSH      cgi 发送 刷新数据请求
 * CGITOMQTT   _1_LEDCTRL  cgi 发送 1号设备led灯控制
 * CGITOMQTT   _2_LEDCTRL   cgi 发送 2号设备led灯控制
 * 
 * 
 * 
 * MQTTTOCGI   FLUSHBACK  MQTT 发送 刷新回复
 * MQTTTOCGI   OK         MQTT 发送  控制成功
 * 
 * CGITOSQLITE3  GETHistoricalRecord cgi 发送 获取历史记录
 * SQLITE3TOCGI  BACKHistoricalRecord sqlite3 发送 历史记录
 * SQLITE3TOCGI  BACKOVERHistoricalRecord sqlite3 发送 历史记录结束
 * 
 * ***********************************************/

extern int msg_id;
extern struct mosquitto *m_hMqtt;
extern void My_Kill(int sig);
extern int MQTT_Init(struct mosquitto *m_hMqtt);
extern void *CGI_CMD_GET_TO_MQTT(void *arg);
extern void *CGI_CMD_GET_TO_SQLITE3(void *arg);
extern void *MyMQTT_INIT(void *arg);

extern void GET_HistoricalRecord();
extern int MQTT_Destroy(struct mosquitto *m_hMqtt);
extern int publish_subscribe(char *topic,char *data);
extern void my_connect_callback(struct mosquitto *mosq, void *obj, int rc);
extern void my_disconnect_callback(struct mosquitto *mosq, void *obj, int result);
extern void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
extern void my_subscribe_callback(struct mosquitto *mosq, void *obj, int mid,int qos_count,const int *granted_qos);
extern int MySqlite3(char *dbname,char *column,char *table,char *condition,int *nRow,int *nColumn,char * errmsg,char ***dbResult,int type);
#endif