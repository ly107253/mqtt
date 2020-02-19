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
		printf("RecvTopic(%s), Len(%d)\r\n", message->topic, message->payloadlen);
    }
	else
	{
        printf("%s (null)\n", message->topic);
    }

}

static void mosq_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
    if(!result){
        /* Subscribe to broker information topics on successful connect. */
		/* Subscribe to the topic. */
        mosquitto_subscribe(mosq, NULL, "#", 2);
	
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

void* msg_mqtt_init(MQTT_CONFIG_T *pCfg)
{
	struct mosquitto *mosq = NULL;

	if(pCfg == NULL)
	{
		return NULL;
	}
	//mosquitto库初始化
	mosquitto_lib_init();
	//创建客户端
	mosq = mosquitto_new(NULL,true,NULL);
	if(mosq == NULL)
	{
        mosquitto_lib_cleanup();
		
        return NULL;
	}
	
	if((pCfg->username || pCfg->password) && mosquitto_username_pw_set(mosq, pCfg->username, pCfg->password)){
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
	
	mosquitto_connect(mosq, pCfg->host, pCfg->port, pCfg->keepalive);        //建立连接
	
	if(MOSQ_ERR_SUCCESS != mosquitto_loop_start(mosq))
	{
		mosquitto_destroy(mosq);
		mosquitto_lib_cleanup();

		return NULL;
	}

	return (void*)mosq;
}

int msg_mqtt_send(void* hMosq,MQTT_MSG_T *pMsg)
{	
	if(pMsg == NULL)
	{
		return -1;
	}
	
	if(mosquitto_publish((struct mosquitto*)hMosq,NULL,pMsg->topic,pMsg->payloadlen,pMsg->payload,0,0) == MOSQ_ERR_SUCCESS)
	{
		return -1;
	}

	return 0;
}

void msg_mqtt_destory(void* hMosq)
{
	if(hMosq != NULL)
	{
		mosquitto_destroy(hMosq);
		mosquitto_lib_cleanup();
	}
}

