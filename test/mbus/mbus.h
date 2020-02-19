#ifndef _MBUS_H__
#define _MBUS_H__

#define MQTT_TOPIC_SIZE       255
#define MSG_MAX_SIZE          128

/**> mqtt≈‰÷√*/
typedef struct mqtt_config {
	int  keepalive;
	char *host;
	int  port;
	char *username;
	char *password;
}MQTT_CONFIG_T;

/**> mqttœ˚œ¢ÃÂ*/
typedef struct mqtt_msg
{
	char* topic;
	int payloadlen;
	void *payload;
}MQTT_MSG_T;

void msg_mqtt_destory(void* hMosq);
void* msg_mqtt_init(MQTT_CONFIG_T *pCfg);
int msg_mqtt_send(void* hMosq,MQTT_MSG_T* pMsg);


#endif
