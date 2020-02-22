#ifndef _MBUS_H__
#define _MBUS_H__

#include "mosquitto.h"

#define MQTT_TOPIC_SIZE       128
#define MSG_MAX_SIZE          2048


/**> mqtt����*/
typedef struct mqtt_config 
{
	char *id;
	int  keepalive;
	char *host;
	int  port;
	char *username;
	char *password;
	bool clean_session;
	char *will_topic;
	char *will_payload;
	long will_payloadlen;
	int  will_qos;
	bool will_retain;
	bool connected;
	
	void (*pfLog)(struct mosquitto *, void *, int, const char *); /**>��־�ص�*/
	void (*pfConnect)(struct mosquitto *, void *, int);           /**>���ӻص�*/
	void (*pfDisconnect)(struct mosquitto *, void *, int);        /**>�Ͽ����ӻص�*/
	void (*pfMessage)(struct mosquitto *, void *, const struct mosquitto_message *); /**>������Ϣ�ص�*/
	void (*pfSubscribe)(struct mosquitto *, void *, int, int, const int *);/**>������Ϣ�ص�*/
}MQTT_CONFIG_T;

/**> mqtt��Ϣ��*/
typedef struct mqtt_msg
{
	char* topic;          
	int payloadlen;
	void *payload;
}MQTT_MSG_T;


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
struct mosquitto* mqtt_client_init(MQTT_CONFIG_T *pCfg);
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
int mqtt_client_connect(struct mosquitto* hMosq,MQTT_CONFIG_T *pCfg);
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
int mqtt_client_disconnect(struct mosquitto* hMosq,MQTT_CONFIG_T *pCfg);
/**
 ******************************************************************************
 * @brief		����MQTT�ͻ��ˣ�����һ���̣߳�һֱ����socket��Ϣ���Ͽ�����Զ�������
 * @param[in]	hMosq  : MQTT�ͻ��˾��
 * @return      
 * @details
 ******************************************************************************
 */
int mqtt_client_loop_start(struct mosquitto* hMosq);
/**
******************************************************************************
* @brief		 ����MQTT�ͻ��ˣ������Զ������̣߳���Ҫ�û��Լ������̣߳�
* @param[in]	 hMosq	: MQTT�ͻ��˾��
* @return 	 
* @details
******************************************************************************
*/
void mqtt_lient_loop_forever(struct mosquitto* hMosq);
/**
******************************************************************************
* @brief		 ����MQTT�ͻ���
* @param[in]	 hMosq	: MQTT�ͻ��˾��
* @return 	 
* @details
******************************************************************************
*/
void mqtt_client_destory(struct mosquitto* hMosq);
/**
******************************************************************************
* @brief		 MQTT�ͻ��˷�����Ϣ
* @param[in]	 hMosq	: MQTT�ͻ��˾��
* @param[in]	 pMsg	: MQTT��Ϣ��
* @return 	 
* @details
******************************************************************************
*/
int mqtt_msg_send(struct mosquitto* hMosq,MQTT_MSG_T *pMsg);


#endif
