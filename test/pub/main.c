#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mbus.h"


int main(int argc, char *argv[])
{
	MQTT_CONFIG_T cfg;
	MQTT_MSG_T    msg;
	void* myHandle;	
	char buf[MSG_MAX_SIZE];

	memset(&cfg,0x00,sizeof(MQTT_CONFIG_T));
	cfg.host = "127.0.0.1";
	cfg.port = 1883;
	cfg.keepalive = 60;
	cfg.username = "admin";
	cfg.password = "admin";
	//≥ı ºªØmqttø‚
	myHandle = msg_mqtt_init(&cfg);
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
		
		if(msg_mqtt_send(myHandle,&msg) == 0)
		{
			break;
		}

		sleep(2);
	}

	msg_mqtt_destory(myHandle);
	
	return 0;
}

