#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mbus.h"


int main(int argc, char *argv[])
{	
	MQTT_CONFIG_T cfg;
	void* myHandle;
	
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
#if 1	
	while(1)
	{
		sleep(2);
	}
#endif
	return 0;
}

