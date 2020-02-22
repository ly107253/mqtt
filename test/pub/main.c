#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mqtt.h"

static MQTT_CONFIG_T s_ClientCfg;

static int mqtt_client_config_load( void )
{
	s_ClientCfg.host = "127.0.0.1";
	s_ClientCfg.port = 1883;
	s_ClientCfg.keepalive = 60;
	s_ClientCfg.username = "admin";
	s_ClientCfg.password = "admin";
	s_ClientCfg.clean_session = true;
	s_ClientCfg.id = "lymqtt_pub";
	
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

