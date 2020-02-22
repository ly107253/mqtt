#ifndef _MBUS_H__
#define _MBUS_H__

#include "mosquitto.h"

#define MQTT_TOPIC_SIZE       255
#define MSG_MAX_SIZE          128


/**> mqtt≈‰÷√*/
typedef struct mqtt_config {
	char *id;
	int  keepalive;
	char *host;
	int  port;
	char *username;
	char *password;
	bool clean_session;
	char *will_topic;
	char *will_payload;
	long will_payloadlen;
	int  will_qos;
	bool will_retain;
}MQTT_CONFIG_T;

/**> mqttœ˚œ¢ÃÂ*/
typedef struct mqtt_msg
{
	char* topic;
	int payloadlen;
	void *payload;
}MQTT_MSG_T;

void mqtt_client_destory(void* hMosq);
void* mqtt_client_init(MQTT_CONFIG_T *pCfg);
int mqtt_msg_send(void* hMosq,MQTT_MSG_T *pMsg);
int mqtt_msg_recv(void* hMosq,MQTT_MSG_T *pMsg);


#endif
