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
	s_ClientCfg.id = "lymqtt_sub";

	return 1;
}


int main(int argc, char *argv[])
{	
	void* myHandle;	
	
	mqtt_client_config_load();

	//≥ı ºªØmqttø‚
	myHandle = mqtt_client_init(&s_ClientCfg);
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

