#ifndef _MBUS_H__
#define _MBUS_H__

#include "mosquitto.h"

#define MQTT_TOPIC_SIZE       128
#define MSG_MAX_SIZE          2048


/**> mqtt配置*/
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
	
	void (*pfLog)(struct mosquitto *, void *, int, const char *); /**>日志回调*/
	void (*pfConnect)(struct mosquitto *, void *, int);           /**>连接回调*/
	void (*pfDisconnect)(struct mosquitto *, void *, int);        /**>断开连接回调*/
	void (*pfMessage)(struct mosquitto *, void *, const struct mosquitto_message *); /**>接收消息回调*/
	void (*pfSubscribe)(struct mosquitto *, void *, int, int, const int *);/**>订阅消息回调*/
}MQTT_CONFIG_T;

/**> mqtt消息体*/
typedef struct mqtt_msg
{
	char* topic;          
	int payloadlen;
	void *payload;
}MQTT_MSG_T;


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
struct mosquitto* mqtt_client_init(MQTT_CONFIG_T *pCfg);
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
int mqtt_client_connect(struct mosquitto* hMosq,MQTT_CONFIG_T *pCfg);
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
int mqtt_client_disconnect(struct mosquitto* hMosq,MQTT_CONFIG_T *pCfg);
/**
 ******************************************************************************
 * @brief		启动MQTT客户端（创建一个线程，一直处理socket信息，断开后会自动重连）
 * @param[in]	hMosq  : MQTT客户端句柄
 * @return      
 * @details
 ******************************************************************************
 */
int mqtt_client_loop_start(struct mosquitto* hMosq);
/**
******************************************************************************
* @brief		 启动MQTT客户端（不会自动创建线程，需要用户自己创建线程）
* @param[in]	 hMosq	: MQTT客户端句柄
* @return 	 
* @details
******************************************************************************
*/
void mqtt_lient_loop_forever(struct mosquitto* hMosq);
/**
******************************************************************************
* @brief		 销毁MQTT客户端
* @param[in]	 hMosq	: MQTT客户端句柄
* @return 	 
* @details
******************************************************************************
*/
void mqtt_client_destory(struct mosquitto* hMosq);
/**
******************************************************************************
* @brief		 MQTT客户端发布信息
* @param[in]	 hMosq	: MQTT客户端句柄
* @param[in]	 pMsg	: MQTT消息体
* @return 	 
* @details
******************************************************************************
*/
int mqtt_msg_send(struct mosquitto* hMosq,MQTT_MSG_T *pMsg);


#endif
