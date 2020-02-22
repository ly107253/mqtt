#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mqtt.h"
/**
 ******************************************************************************
 * @brief		mqtt参数配置
 * @param[in]	hMosq : MQTT客户端句柄
 * @param[in]	cfg	  : MQTT客户端配置
 * @return      成功返回 MOSQ_ERR_SUCCESS 失败返回 1
 * @details
 ******************************************************************************
 */
static int mqtt_client_opts_set(struct mosquitto *hMosq, MQTT_CONFIG_T *pCfg)
{
	if(pCfg->will_topic && mosquitto_will_set(hMosq, pCfg->will_topic,
				pCfg->will_payloadlen, pCfg->will_payload, pCfg->will_qos,
				pCfg->will_retain)){

		printf("Error: Problem setting will.\n");
		mosquitto_lib_cleanup();
		return 1;
	}

	if((pCfg->username || pCfg->password) && mosquitto_username_pw_set(hMosq, pCfg->username, pCfg->password)){
		printf("Error: Problem setting username and/or password.\n");
		mosquitto_lib_cleanup();
		return 1;
	}

	return MOSQ_ERR_SUCCESS;
}
/**
 ******************************************************************************
 * @brief		MQTT客户端初始化
 * @param[in]	pCfg  : MQTT客户端配置
 * @return      
 				hMosq   客户端句柄
 				NULL    初始化失败
 * @details
 ******************************************************************************
 */
struct mosquitto* mqtt_client_init(MQTT_CONFIG_T *pCfg)
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
	mosquitto_log_callback_set(hMosq, pCfg->pfLog);
	/*connect to broken*/
    mosquitto_connect_callback_set(hMosq, pCfg->pfConnect);
	/*disconnect to broken*/
    mosquitto_disconnect_callback_set(hMosq, pCfg->pfDisconnect);
	/*recv sub data*/
    mosquitto_message_callback_set(hMosq, pCfg->pfMessage);
	/*recv sub request*/
    mosquitto_subscribe_callback_set(hMosq, pCfg->pfSubscribe);

	return hMosq;
	
cleanup:
	mosquitto_destroy(hMosq);
	mosquitto_lib_cleanup();
	
	return NULL;
}
/**
 ******************************************************************************
 * @brief		MQTT客户端连接Brocker 
 * @param[in]	hMosq  : MQTT客户端句柄
 * @param[in]	pCfg   : MQTT客户端配置
 * @return      
 				MOSQ_ERR_SUCCESS   连接成功
 * @details
 ******************************************************************************
 */
int mqtt_client_connect(struct mosquitto* hMosq,MQTT_CONFIG_T *pCfg)
{	
	int port;

	if(pCfg->port < 0)
	{
		port = 1883;
	}
	else 
	{
		port = pCfg->port;
	}

	return  mosquitto_connect(hMosq, pCfg->host, port, pCfg->keepalive);
}
/**
 ******************************************************************************
 * @brief		MQTT客户端与Brocker断开连接 
 * @param[in]	hMosq  : MQTT客户端句柄
 * @param[in]	pCfg   : MQTT客户端配置
 * @return      
 				MOSQ_ERR_SUCCESS   断开连接成功
 * @details
 ******************************************************************************
 */
int mqtt_client_disconnect(struct mosquitto* hMosq,MQTT_CONFIG_T *pCfg)
{
	return mosquitto_disconnect(hMosq);
}
/**
 ******************************************************************************
 * @brief		启动MQTT客户端（创建一个线程，一直处理socket信息，断开后会自动重连）
 * @param[in]	hMosq  : MQTT客户端句柄
 * @return      
 * @details
 ******************************************************************************
 */
 int mqtt_client_loop_start(struct mosquitto* hMosq)
{
	return mosquitto_loop_start(hMosq);
}
/**
******************************************************************************
* @brief		 启动MQTT客户端（不会自动创建线程，需要用户自己创建线程）
* @param[in]	 hMosq	: MQTT客户端句柄
* @return 	 
* @details
******************************************************************************
*/
void mqtt_lient_loop_forever(struct mosquitto* hMosq)
{
	 mosquitto_loop_forever(hMosq,-1,1);
}
/**
******************************************************************************
* @brief		 销毁MQTT客户端
* @param[in]	 hMosq	: MQTT客户端句柄
* @return 	 
* @details
******************************************************************************
*/
void mqtt_client_destory(struct mosquitto* hMosq)
{
	if(hMosq != NULL)
	{
		mosquitto_destroy(hMosq);
		mosquitto_lib_cleanup();
	}
}
/**
******************************************************************************
* @brief		 MQTT客户端发布信息
* @param[in]	 hMosq	: MQTT客户端句柄
* @param[in]	 pMsg	: MQTT消息体
* @return 	 
* @details
******************************************************************************
*/
int mqtt_msg_send(struct mosquitto* hMosq,MQTT_MSG_T *pMsg)
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


