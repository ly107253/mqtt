#ifndef _MBUS_H__
#define _MBUS_H__

#define MQTT_TOPIC_SIZE       255
#define MSG_MAX_SIZE          128

void msg_mqtt_destory(void* hSocket);
void* msg_mqtt_init(void);
int msg_mqtt_send(void* hSocket,void* buf,int len);


#endif
