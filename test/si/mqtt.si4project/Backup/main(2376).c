#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mbus.h"


int main(int argc, char *argv[])
{
	HANDLE myHandle;	
	
	//≥ı ºªØmqttø‚
	myHandle = msg_mqtt_init();
	if(myHandle == NULL)
	{
		return -1;
	}
	
	while(1)
	{
		sleep(2);
	}
	
	return 0;
}

