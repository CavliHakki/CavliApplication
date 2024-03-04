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

/* C16QS modulunu tek bir IP adresine TCP baglantisi yapilacaginin
 * bildirilmesi icin kullanilan fonksiyon */
ErrorStatus CIPMUX_setting(void) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	C16QS_Serial_Flush_Queue(&C16QS_Buf);

	sprintf(buf, "AT+CIPMUX=0\r\n");

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(100);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_CIPMUX_OK_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;

}

ErrorStatus Control_CIPMUX_OK_Message(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

/* Sunucu tarafinda dinlemeye alinmis olan bir porta IP ve port
 * numarasini kullanarak AT komut portu
 * uzerinden bu fonsiyon ile baglanti kurulur.*/
ErrorStatus CIPSTART_setting(char *IP, uint16_t Port) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	sprintf(buf, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", IP, Port);

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(10000);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_CIPSTART_response(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_CIPSTART_response(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\n\r\n+CIPSTART: CONNECT OK\r\n\r\nOK\r\n",
			strlen("\r\n\r\n+CIPSTART: CONNECT OK\r\n\r\nOK\r\n"));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

/* Baglantinin durumunu herhangi bir zamanda kontrol edebilirsiniz.
 * Bunun icin AT komut portu uzerinden bu fonksiyon kullanilir.*/
ErrorStatus Control_TCP_connection(void) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	sprintf(buf, "AT+CIPSTATUS\r\n");

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(500);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_TCP_Connection_Message(rxBuf, DestinationIP, DestinationPort)
			== ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_TCP_Connection_Message(char *buf, char *IP, uint16_t Port) {
	char tbuf[50];
	int result = -1;

	Clear_Buffer(tbuf, 50);

	sprintf(tbuf, "\r\n\r\nC:0,TCP,%s,%d,CONNECTED\r\n\r\nOK\r\n", IP, Port);

	result = strncmp(buf, tbuf, strlen(tbuf));
	if (result == 0)
		return SUCCESS;

	return ERROR;

}

/* Baglanti kurdugunuz TCP portuna belirli sayida
 * veri gondermek icin kullanilan fonksiyon */
ErrorStatus Send_Data_to_TCP(char *message, uint16_t length) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	sprintf(buf, "AT+CIPSEND=%d\r\n", length);

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(500);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_Data_Enter(rxBuf) == ERROR)
		return ERROR;

	Clear_Buffer(rxBuf, 100);
	sprintf(buf, "%s", message);

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);

	HAL_Delay(100);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_Send_OK(rxBuf) == ERROR)
		return ERROR;
	return SUCCESS;

}

ErrorStatus Control_Data_Enter(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\n\r\n> ", strlen("\r\n\r\n> "));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

ErrorStatus Control_Send_OK(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\n+CIPSEND: SEND OK\r\n",
			strlen("\r\n+CIPSEND: SEND OK\r\n"));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

ErrorStatus Send_Data_to_TCP_2(char *message) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	sprintf(buf, "AT+CIPSEND\r\n");

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(500);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_Data_Enter(rxBuf) == ERROR)
		return ERROR;

	sprintf(buf, "%s", message);

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);

	Clear_Buffer(rxBuf, 100);

	/* veri gondermeyi bitşrmek icin exit komutu */
	buf[0] = 0x1a; // ctrl+z
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, 1, 10);

	HAL_Delay(100);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_Send_OK(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

/* Acik olan TCP portunu kapatmak icin kullanilan fonksiyon */
ErrorStatus TCP_close(void) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);
	C16QS_Serial_Flush_Queue(&C16QS_Buf);

	sprintf(buf, "AT+CIPCLOSE\r\n");

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(500);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_TCP_Close_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_TCP_Close_Message(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\n\r\nCLOSE OK\r\n\r\nOK\r\n",
			strlen("\r\n\r\nCLOSE OK\r\n\r\nOK\r\n"));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

/*Tum TCP wireless baglantilarinin kapatilmasi icin kullanilan fonksiyondur.*/
ErrorStatus All_TCP_close(void) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	sprintf(buf, "AT+CIPSHUT\r\n");

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(100);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_All_TCP_Close_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_All_TCP_Close_Message(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\n\r\nSHUT OK\r\n\r\nOK\r\n",
			strlen("\r\n\r\nSHUT OK\r\n\r\nOK\r\n"));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

/* TCP NORMAL MOD UYGULAMASI */
void C16QS_TCP_Sequence(void) {
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
				StepStatus = step_pwrkey;
				break;
			} else {
				HAL_Delay(2000);
				StepStatus = step_internet_control;
				break;
			}
		}
		StepStatus = step_cipmux_setting;
		break;

	case step_cipmux_setting:
		if (CIPMUX_setting() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			try_counter =0;
			StepStatus = step_cipstart;
			break;
		}
		break;

	case step_cipstart:
		if (CIPSTART_setting(DestinationIP, DestinationPort) == ERROR) {
			try_counter++;
			if (try_counter == 10) {
				StepStatus = step_pwrkey;
				break;
			} else {
				HAL_Delay(2000);
				StepStatus = step_cipstart;
				break;
			}
		} else {
			StepStatus = step_Control_TCP_connection;
			break;
		}
		break;

	case step_Control_TCP_connection:
		if (Control_TCP_connection() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_Send_data_to_TCP;
			break;
		}
		break;

		/* Belirli sayida veri gonderme */
	case step_Send_data_to_TCP:
		if (Send_Data_to_TCP("Merhaba Cavli Wireless", 22) == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_Send_data_to_TCP_2;
			break;
		}
		break;

	case step_Send_data_to_TCP_2:
		if (Send_Data_to_TCP_2("Merhaba Cavli Wireless") == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_Close_TCP;
			HAL_Delay(1000);
			break;
		}
		break;

	case step_Close_TCP:
		if (TCP_close() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_All_TCP_Close;
			break;
		}
		break;

	case step_All_TCP_Close:
		if (All_TCP_close() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_listen;
			break;
		}
		break;

	case step_listen:
		HAL_Delay(10);
		StepStatus = step_listen;
		break;

	default:
		break;
	}

}