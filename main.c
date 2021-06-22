
/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @LastEditTime: 2021-06-22 08:54:12
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include <stdio.h>
#include <stdint.h>

#include "mqttclient.h"
#include "log.h"
#include "cJSON.h"

//static char mqtt_msg[256] = {'\0'};
void msg_gen(char **msg_json){
    int age = 18;
    char *name = "student";
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", name);
    cJSON_AddNumberToObject(root, "age", age);
    *msg_json = cJSON_Print(root);
    LOG(DEBUG, "msg_json is [%s]", *msg_json);
    strncpy(*msg_json, cJSON_Print(root), sizeof(*msg_json));
    //LOG(DEBUG, "msg_json is [%s]", *msg_json);
}
static int mqtt_publish_handle1(mqtt_client_t *client)
{
    char *msg_json;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    //msg_gen(&mqtt_msg);
    int age = 18;
    char *name = "student";
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", name);
    cJSON_AddNumberToObject(root, "age", age);
    msg_json = cJSON_Print(root);
    LOG(DEBUG, "msg_json is [%s]", msg_json);

    msg.qos = 0;
    msg.payload = (void *) msg_json;

    return mqtt_publish(client, "zhangtaoTX", &msg);
}
void recv_handlel_bk(void* client, message_data_t* msg){
    (void) client;
    LOG(INFO,"----------------------------------------------------------------------------------");
    LOG(INFO,"get message");
    LOG(DEBUG, "msg->message->payload is [%s]", (char *)msg->message->payload);
    cJSON *root = cJSON_Parse(msg->message->payload);
    /*
     { "message":"hellow client", "code":10086}
    */
    if(!root){
        LOG(ERROR, "not a json format:[%s]", (char *)msg->message->payload);
        return;
    }
    char *message  = cJSON_GetObjectItem(root, "message")->valuestring;
    int code = -1;
    code = cJSON_GetObjectItem(root, "code")->valueint;
    if(message){
        LOG(DEBUG,"message is [%s]", message);  
    }else{
        LOG(ERROR, "get message fail");
    }
    if(code != -1){
        LOG(DEBUG,"code is [%d]", code);  
    }else{
        LOG(ERROR, "get code fail");
    }
    LOG(INFO,"----------------------------------------------------------------------------------");
    cJSON_Delete(root);
}

int main(void)
{
    log_init();
    LOG(INFO, "Program start");
    mqtt_client_t *client = NULL;

    //mqtt_log_init();

    client = mqtt_lease();

    mqtt_set_host(client, "iot.geniatech.com");
    mqtt_set_port(client, "1883");
    mqtt_set_clean_session(client, 1);

    mqtt_connect(client);
    mqtt_subscribe(client, "zhangtaoRX", QOS0, recv_handlel_bk);

    while (1) {
        mqtt_publish_handle1(client);
        mqtt_sleep_ms(4 * 1000);
    }
}
