/*
 * C16QS.h
 *
 *  Created on: Jan 26, 2024
 *      Author: hk
 */

#ifndef C16QS_H_
#define C16QS_H_
#include "main.h"
#include "Serial.h"
#include "stdio.h"
#include "string.h"

/* MQTT Broker Informations */
#define BrokerAddress			"broker.test.net"
#define BrokerPortNumner		"1883"
#define UserName				"UserName"
#define Password				"123456"
#define ClientID				"TEST"
#define LastWillTopic			"LWT"
#define LastWillMessage			"LWM"
#define DestinationTopic		"TestTopic"
#define SubscribeTopic			"SubTopic"

extern uint8_t SessionID;

extern UART_HandleTypeDef huart1;

typedef enum {
	Off = 0, On = 1
} Status_e;

typedef enum {
	step_pwrkey = 0,
	step_rst,
	step_echo_dis,
	step_alive_control,
	step_func_control,
	step_func_enable,
	step_NetworkReg,
	step_8,
	step_internet_control,
	step_MqttCreateSession,
	step_MqttConnection,
	step_sendMessageTopic,
	step_SubScribe,
	step_Listen
} Step_e;

extern Step_e StepStatus;

void PowerKeyControl(Status_e sts);
void PowerKeyFunction(void);

void RstKeyControl(Status_e sts);
void ResetKeyFunction(void);

void Clear_Buffer(char *buf, uint16_t len);
void Echo_Disable(void);
ErrorStatus Alive_Control(void);
ErrorStatus Control_OK_Message(char *buf);

ErrorStatus Modul_Function_Control(void);
ErrorStatus Control_FunctionEnable_Message(char *buf);

ErrorStatus Modul_Function_Enable(void);

ErrorStatus Network_Register_Control(void);
ErrorStatus Control_NetworReg_Message(char *buf);

ErrorStatus Internet_Control(void);
ErrorStatus Control_Internet_Message(char *buf);

ErrorStatus MQTT_Create_Session(void);
ErrorStatus Control_CreateSession_Message(char *buf);

ErrorStatus MQTT_Connection(void);
ErrorStatus Control_Connection_Message(char *buf);

ErrorStatus Send_Message_Topic(void);
ErrorStatus Control_Send_Message(char *buf);

ErrorStatus Set_Subscribe_Topic(void);
ErrorStatus Control_Subscribe_Message(char *buf);

void C16QS_Sequence(void);

#endif /* C16QS_H_ */
