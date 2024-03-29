/*
 * C16QS.c
 *
 *  Created on: Jan 26, 2024
 *      Author: hk
 */
#include "C16QS.h"

Step_e StepStatus = step_pwrkey;
uint8_t SessionID = 0;

void PowerKeyControl(Status_e sts) {
	if (sts == On) {
		HAL_GPIO_WritePin(PowerKey_GPIO_Port, PowerKey_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(PowerKey_GPIO_Port, PowerKey_Pin, GPIO_PIN_RESET);
	}
}

void PowerKeyFunction(void) {
	PowerKeyControl(On);
	HAL_Delay(100);
	PowerKeyControl(Off);
}

void RstKeyControl(Status_e sts) {
	if (sts == On) {
		HAL_GPIO_WritePin(RstKey_GPIO_Port, RstKey_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(RstKey_GPIO_Port, RstKey_Pin, GPIO_PIN_RESET);
	}
}

void ResetKeyFunction(void) {
	RstKeyControl(On);
	HAL_Delay(100);
	RstKeyControl(Off);
}

void Echo_Disable(void) {
	char buf[20];
	uint16_t size = 0;
	sprintf(buf, "ATE0\r\n");
	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(500);
}

void Clear_Buffer(char *buf, uint16_t len) {
	uint16_t i = 0;
	for (i = 0; i < len; i++) {
		buf[i] = 0;
	}
}

ErrorStatus Alive_Control(void) {
	char buf[20];
	char rxBuf[20];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 20);
	Clear_Buffer(rxBuf, 20);

	sprintf(buf, "AT\r\n");
	size = strlen(buf);
	C16QS_Serial_Flush_Queue(&C16QS_Buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(500);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_OK_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_OK_Message(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\nOK\r\n", strlen("\r\nOK\r\n"));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

ErrorStatus Modul_Function_Control(void) {
	char buf[20];
	char rxBuf[50];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 20);
	Clear_Buffer(rxBuf, 50);

	sprintf(buf, "AT+CFUN?\r\n");
	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(500);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_FunctionEnable_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_FunctionEnable_Message(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\n+CFUN:1\r\n\r\nOK\r\n",
			strlen("\r\n+CFUN:1\r\n\r\nOK\r\n"));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

ErrorStatus Modul_Function_Enable(void) {
	char buf[20];
	char rxBuf[50];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 20);
	Clear_Buffer(rxBuf, 50);

	sprintf(buf, "AT+CFUN=1\r\n");
	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(500);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_OK_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Network_Register_Control(void) {
	char buf[20];
	char rxBuf[50];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 20);
	Clear_Buffer(rxBuf, 50);

	sprintf(buf, "AT+CEREG?\r\n");
	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(500);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_NetworReg_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_NetworReg_Message(char *buf) {
	int result = -1;

	result = strncmp(buf, "\r\n+CEREG: 0,1\r\n\r\nOK\r\n",
			strlen("\r\n+CEREG: 0,1\r\n\r\nOK\r\n"));
	if (result == 0)
		return SUCCESS;

	result = strncmp(buf, "\r\n+CEREG: 0,5\r\n\r\nOK\r\n",
			strlen("\r\n+CEREG: 0,5\r\n\r\nOK\r\n"));

	return ERROR;
}

ErrorStatus Internet_Control(void) {
	char buf[20];
	char rxBuf[50];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 20);
	Clear_Buffer(rxBuf, 50);

	sprintf(buf, "AT+CGACT?\r\n");
	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(500);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_Internet_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_Internet_Message(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\n+CGACT: 1,1\r\n", strlen("\r\n+CGACT: 1,1\r\n"));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

ErrorStatus MQTT_Create_Session(void) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	sprintf(buf,
			"AT+MQTTCREATE=\"%s\",%s,\"%s\",250,0,\"%s\",\"%s\",\"%s\",\"%s\",2,0\r\n",
			BrokerAddress, BrokerPortNumner, ClientID, UserName, Password,
			LastWillTopic, LastWillMessage);

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(1000);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_CreateSession_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;

}

ErrorStatus Control_CreateSession_Message(char *buf) {
	int result = -1;
	uint8_t indeks = 0;
	uint8_t len = strlen("\r\n\r\n+MQTTCREATE:");
	result = strncmp(buf, "\r\n\r\n+MQTTCREATE:", len);
	indeks = len;
	if (result == 0) {
		if (buf[indeks] == ' ') {
			indeks++;
			SessionID = buf[indeks];
			return SUCCESS;
		}
	}
	return ERROR;
}

ErrorStatus MQTT_Connection(void) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	sprintf(buf, "AT+MQTTCONN=%c,0\r\n", SessionID);

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(1000);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_Connection_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_Connection_Message(char *buf) {
	char rbuf[200];
	int result = -1;
	uint8_t len = 0;

	Clear_Buffer(rbuf, 200);

	sprintf(rbuf,
			"\r\n\r\n+MQTTCONN: %c: CONNECTING\r\n\r\n+MQTTCONN: %c: CONNECTED,0\r\n\r\nOK\r\n",
			SessionID, SessionID);
	len = strlen(buf);

	result = strncmp(buf, rbuf, len);

	if (result == 0) {
		return SUCCESS;
	}
	return ERROR;
}

ErrorStatus Send_Message_Topic(void) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	sprintf(buf, "AT+MQTTPUB=%c,\"%s\",\"Merhaba MQTT Broker\",0,0,0,1\r\n",
			SessionID, DestinationTopic);

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(2000);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_Send_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_Send_Message(char *buf) {
	char rbuf[200];
	int result = -1;
	uint8_t len = 0;

	Clear_Buffer(rbuf, 200);

	sprintf(rbuf,
			"\r\n\r\n+MQTTPUB: %c: PUBLISHING\r\n\r\n+MQTTPUB: %c: PUBLISH SUCCESS\r\n\r\nOK\r\n",
			SessionID, SessionID);
	len = strlen(buf);

	result = strncmp(buf, rbuf, len);

	if (result == 0) {
		return SUCCESS;
	}
	return ERROR;
}

ErrorStatus Set_Subscribe_Topic(void) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	C16QS_Serial_Flush_Queue(&C16QS_Buf);

	sprintf(buf, "AT+MQTTSUBUNSUB=%c,\"%s\",1,0\r\n", SessionID,
	SubscribeTopic);

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(1000);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_Subscribe_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_Subscribe_Message(char *buf) {
	char rbuf[200];
	int result = -1;
	uint8_t len = 0;

	Clear_Buffer(rbuf, 200);

	sprintf(rbuf,
			"\r\n\r\n+MQTTSUBUNSUB: %c: SUBSCRIBING\r\n\r\n+MQTTSUBUNSUB: %c: SUBSCRIBE SUCCESS\r\n\r\nOK\r\n",
			SessionID, SessionID);



	len = strlen(buf);

	result = strncmp(buf, rbuf, len);

	if (result == 0) {
		return SUCCESS;
	}
	return ERROR;
}

void C16QS_Sequence(void) {
	uint8_t try_counter = 0;

	switch (StepStatus) {
	case step_pwrkey:
		PowerKeyFunction();
		HAL_Delay(500);
		StepStatus = step_rst;
		break;

	case step_rst:
		ResetKeyFunction();
		StepStatus = step_echo_dis;
		break;

	case step_echo_dis:
		Echo_Disable();
		StepStatus = step_alive_control;
		break;

	case step_alive_control:
		if (Alive_Control() == ERROR)
			StepStatus = step_pwrkey;
		else
			StepStatus = step_func_control;
		break;

	case step_func_control:
		if (Modul_Function_Control() == ERROR)
			StepStatus = step_func_enable;
		else
			StepStatus = step_NetworkReg;
		break;

	case step_func_enable:
		if (Modul_Function_Enable() == ERROR)
			StepStatus = step_pwrkey;
		else
			StepStatus = step_NetworkReg;
		break;

	case step_NetworkReg:
		if (Network_Register_Control() == ERROR) {
			try_counter++;
			if (try_counter == 10) {
				StepStatus = step_pwrkey;
				break;
			} else {
				HAL_Delay(2000);
				StepStatus = step_NetworkReg;
				break;
			}
		} else {
			try_counter = 0;
			StepStatus = step_internet_control;
			break;
		}
		break;

	case step_internet_control:
		if (Internet_Control() == ERROR) {
			try_counter++;
			if (try_counter == 10) {
				HAL_Delay(2000);
				StepStatus = step_pwrkey;
				break;
			} else {
				try_counter = 0;
				StepStatus = step_internet_control;
				break;
			}
		}
		StepStatus = step_MqttCreateSession;
		break;

	case step_MqttCreateSession:
		if (MQTT_Create_Session() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_MqttConnection;
			break;
		}
		break;

	case step_MqttConnection:
		if (MQTT_Connection() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_sendMessageTopic;
			break;
		}
		break;

	case step_sendMessageTopic:
		if (Send_Message_Topic() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			HAL_Delay(5000);
			StepStatus = step_SubScribe;
			break;
		}
		break;

	case step_SubScribe:
		if (Set_Subscribe_Topic() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_Listen;
			break;
		}
		break;

	case step_Listen:
		/* gelen veri burada islenecektir */
		StepStatus = step_Listen;
		break;

	default:
		break;
	}

}
