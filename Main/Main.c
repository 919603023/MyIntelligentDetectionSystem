
#include "MyFunction.h"

int main(int argc, char const *argv[])
{
    //数据库信息

   
    signal(SIGINT, My_Kill); //注册ctrl+c信号

    key_t key = ftok("My_server", 2020);
    msg_id = msgget(key, IPC_CREAT | 0666);

    pthread_t MQTT_INIT, cgi_cmd_get_to_mqtt,cgi_cmd_get_to_sqlite3;
    pthread_create(&MQTT_INIT, NULL, MyMQTT_INIT, NULL);

    pthread_create(&cgi_cmd_get_to_mqtt, NULL, CGI_CMD_GET_TO_MQTT, NULL);

    pthread_create(&cgi_cmd_get_to_sqlite3,NULL,CGI_CMD_GET_TO_SQLITE3,NULL);
    while (1)

    {
        sleep(1);
        // publish_subscribe("mqtttoserver","niha");
    }
}
