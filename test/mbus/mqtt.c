#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mqtt.h"


/**< log*/
static void mosq_log_callback(struct mosquitto *hMosq, void *userdata, int level, const char *str)
{
    /* Pring all log messages regardless of level. */ 
}

/**< recv*/
static void mosq_message_callback(struct mosquitto *hMosq, void *userdata, const struct mosquitto_message *message)
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

static void mosq_connect_callback(struct mosquitto *hMosq, void *userdata, int result)
{
    if(!result){
        /* Subscribe to broker information topics on successful connect. */
		/* Subscribe to the topic. */
        mosquitto_subscribe(hMosq, NULL, "#", 2);
	
        printf("Connect success\n");
    }
	else
	{
        printf("Connect failed\n");
    }
}

void mosq_subscribe_callback(struct mosquitto *hMosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    /*sub top success*/
}

static int mqtt_client_opts_set(struct mosquitto *hMosq, MQTT_CONFIG_T *cfg)
{
	if(cfg->will_topic && mosquitto_will_set(hMosq, cfg->will_topic,
				cfg->will_payloadlen, cfg->will_payload, cfg->will_qos,
				cfg->will_retain)){

		printf("Error: Problem setting will.\n");
		mosquitto_lib_cleanup();
		return 1;
	}

	if((cfg->username || cfg->password) && mosquitto_username_pw_set(hMosq, cfg->username, cfg->password)){
		printf("Error: Problem setting username and/or password.\n");
		mosquitto_lib_cleanup();
		return 1;
	}

	return MOSQ_ERR_SUCCESS;
}

void* mqtt_client_init(MQTT_CONFIG_T *pCfg)
{
	struct mosquitto *hMosq = NULL;

	if(pCfg == NULL)
	{
		return NULL;
	}
	//mosquitto库初始化
	mosquitto_lib_init();
	
	//创建客户端
	hMosq = mosquitto_new(pCfg->id,pCfg->clean_session,pCfg);
	if(hMosq == NULL){
		goto cleanup;
	}
	//mosq客户端配置
	if(mqtt_client_opts_set(hMosq, pCfg)){
		goto cleanup;
	}
	
	/*log*/
	mosquitto_log_callback_set(hMosq, mosq_log_callback);
	/*connect to broken*/
    mosquitto_connect_callback_set(hMosq, mosq_connect_callback);
	/*recv sub data*/
    mosquitto_message_callback_set(hMosq, mosq_message_callback);
	/*recv sub request*/
    mosquitto_subscribe_callback_set(hMosq, mosq_subscribe_callback);
	
	mosquitto_connect(hMosq, pCfg->host, pCfg->port, pCfg->keepalive);        //建立连接
	
	if(MOSQ_ERR_SUCCESS != mosquitto_loop_start(hMosq)){
		goto cleanup;
	}
	
	return (void*)hMosq;
	
cleanup:
	mosquitto_destroy(hMosq);
	mosquitto_lib_cleanup();
	
	return NULL;
}

void mqtt_client_destory(void* hMosq)
{
	if(hMosq != NULL)
	{
		mosquitto_destroy(hMosq);
		mosquitto_lib_cleanup();
	}
}

int mqtt_msg_recv(void* hMosq,MQTT_MSG_T *pMsg)
{
	
	return 0;
}

int mqtt_msg_send(void* hMosq,MQTT_MSG_T *pMsg)
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


