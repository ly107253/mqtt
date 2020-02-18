#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mbus.h"


int main(int argc, char *argv[])
{
	void* myHandle;	
	char buf[MSG_MAX_SIZE];
	//≥ı ºªØmqttø‚
	myHandle = msg_mqtt_init();
	if(myHandle == NULL)
	{
		return -1;
	}
	
	while(fgets(buf, MSG_MAX_SIZE, stdin) != NULL)
	{		
		if(msg_mqtt_send(myHandle,buf,strlen(buf)+1) == 0)
		{
			break;
		}

		sleep(2);
	}

	msg_mqtt_destory(myHandle);
	
	return 0;
}

