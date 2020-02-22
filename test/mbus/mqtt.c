#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mqtt.h"
/**
 ******************************************************************************
 * @brief		mqtt��������
 * @param[in]	hMosq : MQTT�ͻ��˾��
 * @param[in]	cfg	  : MQTT�ͻ�������
 * @return      �ɹ����� MOSQ_ERR_SUCCESS ʧ�ܷ��� 1
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
 * @brief		MQTT�ͻ��˳�ʼ��
 * @param[in]	pCfg  : MQTT�ͻ�������
 * @return      
 				hMosq   �ͻ��˾��
 				NULL    ��ʼ��ʧ��
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
	//mosquitto���ʼ��
	mosquitto_lib_init();
	
	//�����ͻ���
	hMosq = mosquitto_new(pCfg->id,pCfg->clean_session,pCfg);
	if(hMosq == NULL){
		goto cleanup;
	}
	//mosq�ͻ�������
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
 * @brief		MQTT�ͻ�������Brocker 
 * @param[in]	hMosq  : MQTT�ͻ��˾��
 * @param[in]	pCfg   : MQTT�ͻ�������
 * @return      
 				MOSQ_ERR_SUCCESS   ���ӳɹ�
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
 * @brief		MQTT�ͻ�����Brocker�Ͽ����� 
 * @param[in]	hMosq  : MQTT�ͻ��˾��
 * @param[in]	pCfg   : MQTT�ͻ�������
 * @return      
 				MOSQ_ERR_SUCCESS   �Ͽ����ӳɹ�
 * @details
 ******************************************************************************
 */
int mqtt_client_disconnect(struct mosquitto* hMosq,MQTT_CONFIG_T *pCfg)
{
	return mosquitto_disconnect(hMosq);
}
/**
 ******************************************************************************
 * @brief		����MQTT�ͻ��ˣ�����һ���̣߳�һֱ����socket��Ϣ���Ͽ�����Զ�������
 * @param[in]	hMosq  : MQTT�ͻ��˾��
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
* @brief		 ����MQTT�ͻ��ˣ������Զ������̣߳���Ҫ�û��Լ������̣߳�
* @param[in]	 hMosq	: MQTT�ͻ��˾��
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
* @brief		 ����MQTT�ͻ���
* @param[in]	 hMosq	: MQTT�ͻ��˾��
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
* @brief		 MQTT�ͻ��˷�����Ϣ
* @param[in]	 hMosq	: MQTT�ͻ��˾��
* @param[in]	 pMsg	: MQTT��Ϣ��
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


