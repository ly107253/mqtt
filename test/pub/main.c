#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mqtt.h"

static MQTT_CONFIG_T s_ClientCfg;

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

static void mosq_disconnect_callback(struct mosquitto *hMosq, void *userdata, int result)
{	
	printf("disconnect\n");
}

void mosq_subscribe_callback(struct mosquitto *hMosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    /*sub top success*/
}

static int mqtt_client_config_load( void )
{
	s_ClientCfg.host = "127.0.0.1";
	s_ClientCfg.port = 1883;
	s_ClientCfg.keepalive = 60;
	s_ClientCfg.username = "admin";
	s_ClientCfg.password = "admin";
	s_ClientCfg.clean_session = true;
	s_ClientCfg.id = "lymqtt_pub";

	
	
	s_ClientCfg.pfLog        = mosq_log_callback;	
	s_ClientCfg.pfConnect    = mosq_connect_callback;	
	s_ClientCfg.pfDisconnect = mosq_disconnect_callback;	
	s_ClientCfg.pfMessage    = mosq_message_callback;	
	s_ClientCfg.pfSubscribe  = mosq_subscribe_callback;
	
	return 1;
}

int main(int argc, char *argv[])
{
	MQTT_MSG_T    msg;
	void* myHandle;	
	char buf[MSG_MAX_SIZE];
	
	mqtt_client_config_load();

	//≥ı ºªØmqttø‚
	myHandle = mqtt_client_init(&s_ClientCfg);
	if(myHandle == NULL)
	{
		return -1;
	}
	
	mqtt_client_connect(myHandle, &s_ClientCfg);

	mqtt_client_loop_start(myHandle);
	
	while(fgets(buf, MSG_MAX_SIZE, stdin) != NULL)
	{		
		memset(&msg,0x00,sizeof(MQTT_MSG_T));
		
		msg.topic = "jun";
		msg.payload = (void*)buf;
		msg.payloadlen = strlen(buf)+1;
		
		if(mqtt_msg_send(myHandle,&msg) == 0)
		{
			break;
		}

		sleep(2);
	}

	mqtt_client_destory(myHandle);
	
	return 0;
}

