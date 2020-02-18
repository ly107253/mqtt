#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mbus.h"
#include "mosquitto.h"


/**< log*/
static void mosq_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
    /* Pring all log messages regardless of level. */ 
}

/**< recv*/
static void mosq_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if(message->payloadlen)
	{
		printf("RecvTopic(%s), Msg(%s) Len(%d)\r\n", message->topic, (char*)message->payload,message->payloadlen);
    }
	else
	{
        printf("%s (null)\n", message->topic);
    }
}

static void mosq_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	char topic[MQTT_TOPIC_SIZE] = {0};

    if(!result){
        /* Subscribe to broker information topics on successful connect. */
		/* Subscribe to the topic. */
		sprintf(topic, "%s", "jun");
        mosquitto_subscribe(mosq, NULL, topic, 2);
	
        printf("Connect success\n");
    }
	else
	{
        printf("Connect failed\n");
    }
}

void mosq_subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    /*sub top success*/
}

void* msg_mqtt_init(void)
{
	struct mosquitto *mosq = NULL;

	//mosquitto库初始化
	mosquitto_lib_init();
	//创建客户端
	mosq = mosquitto_new(NULL,true,NULL);
	if(mosq == NULL)
	{
        mosquitto_lib_cleanup();
		
        return NULL;
	}

	/*log*/
	mosquitto_log_callback_set(mosq, mosq_log_callback);
	/*connect to broken*/
    mosquitto_connect_callback_set(mosq, mosq_connect_callback);
	/*recv sub data*/
    mosquitto_message_callback_set(mosq, mosq_message_callback);
	/*recv sub request*/
    mosquitto_subscribe_callback_set(mosq, mosq_subscribe_callback);
	
	mosquitto_connect(mosq, "127.0.0.1", 1883, 60);        //建立连接
	
	if(MOSQ_ERR_SUCCESS != mosquitto_loop_start(mosq))
	{
		mosquitto_destroy(mosq);
		mosquitto_lib_cleanup();

		return NULL;
	}

	return (void*)mosq;
}

int msg_mqtt_send(void* hSocket,void* buf,int len)
{	
	char topic[MQTT_TOPIC_SIZE] = {0};
	
	sprintf(topic, "%s","jun");	
	
	if(mosquitto_publish((struct mosquitto*)hSocket,NULL,topic,len,buf,0,0) == MOSQ_ERR_SUCCESS)
	{
		return 1;
	}

	return 0;
}

void msg_mqtt_destory(void* hSocket)
{
	if(hSocket != NULL)
	{
		mosquitto_destroy(hSocket);
		mosquitto_lib_cleanup();
	}
}

