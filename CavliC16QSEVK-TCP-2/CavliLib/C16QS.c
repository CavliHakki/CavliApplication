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

/* C16QS modulun transparan moda ayarlanmasi icin kullanilan fonksiyondur
 * Parametre aciklamasi:
 * AT+CIPMODE=1,1,0
 * 1	Transparan mod
 * 1	URC disable
 * 0	Server no limit*/
ErrorStatus Transparan_setting(void) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	sprintf(buf, "AT+CIPMODE=1,1,0\r\n");

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(100);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_Transparan_OK_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_Transparan_OK_Message(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\n\r\nOK\r\n", strlen("\r\n\r\nOK\r\n"));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

/* C16QS modulun transparan modda olup olmadigini kontrol
 *  icin kullanilan fonksiyondur.
 *
 * Parametre aciklamasi:
 * AT+CIPMODE=1,1,0
 * 1	Transparan mod
 * 1	URC disable
 * 0	Server no limit*/
ErrorStatus Control_transparan_setting(void) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	sprintf(buf, "AT+CIPMODE?\r\n");

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(100);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_transparan_setting_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_transparan_setting_Message(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\n\r\n+CIPMODE: 1,1,0\r\n\r\nOK\r\n",
			strlen("\r\n\r\n+CIPMODE: 1,1,0\r\n\r\nOK\r\n"));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

/* Sunucu tarafinda dinlemeye alinmis olan bir porta IP ve port
 * numarasini kullanarak AT komut portu
 * uzerinden bu fonsiyon ile baglanti kurulur.*/
ErrorStatus CIPSTART_transparan_setting(char *IP, uint16_t Port) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);
	C16QS_Serial_Flush_Queue(&C16QS_Buf);

	sprintf(buf, "AT+CIPSTART=\"TCP\",%s,%d\r\n", IP, Port);

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 100);
	HAL_Delay(5000);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_Data_Enter(rxBuf) == ERROR)
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

ErrorStatus Control_CIPSTART_response(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\n\r\n+CIPSTART: CONNECT OK\r\n\r\nOK\r\n",
			strlen("\r\n\r\n+CIPSTART: CONNECT OK\r\n\r\nOK\r\n"));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

ErrorStatus Control_Send_OK(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\n\r\n+CIPSEND: SEND OK\r\n",
			strlen("\r\n\r\n+CIPSEND: SEND OK\r\n"));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

void Send_data_to_transparan(char *data) {
	HAL_UART_Transmit(&huart1, (uint8_t*) data, strlen(data), 10);
}

ErrorStatus Exit_transparan_mode(void) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	buf[0] = 0x1a; // ctrl+z exit kodu
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, 1, 10);
	HAL_Delay(100);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_Exit_Message(rxBuf) == ERROR)
		return ERROR;

	return SUCCESS;
}

ErrorStatus Control_Exit_Message(char *buf) {
	int result = -1;
	result = strncmp(buf, "\r\nEXIT OK\r\n", strlen("\r\nEXIT OK\r\n"));
	if (result == 0)
		return SUCCESS;

	return ERROR;
}

ErrorStatus Repeate_enter_transparan_mode(void) {
	char buf[200];
	char rxBuf[100];
	uint8_t i = 0;
	uint16_t size = 0;

	Clear_Buffer(buf, 200);
	Clear_Buffer(rxBuf, 100);

	sprintf(buf, "AT+CIPSEND\r\n");

	size = strlen(buf);
	HAL_UART_Transmit(&huart1, (uint8_t*) buf, size, 10);
	HAL_Delay(100);

	size = C16QS_Serial_Get_Item_Count_in_Queue(&C16QS_Buf);
	for (i = 0; i < size; i++) {
		rxBuf[i] = C16QS_Get_Data_Byte();
	}

	if (Control_Data_Enter(rxBuf) == ERROR)
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

/* TCP TRANSPARAN MOD UYGULAMASI */
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
			StepStatus = step_transparan_setting;
			break;
		}
		break;

	case step_transparan_setting:
		if (Transparan_setting() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_control_transparan_setting;
			break;
		}
		break;

	case step_control_transparan_setting:
		if (Control_transparan_setting() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_start_transparan_connection;
			break;
		}
		break;

	case step_start_transparan_connection:
		if (CIPSTART_transparan_setting(DestinationIP, DestinationPort)
				== ERROR) {
			try_counter++;
			if (try_counter == 10) {
				StepStatus = step_pwrkey;
				break;
			} else {
				HAL_Delay(2000);
				StepStatus = step_start_transparan_connection;
				break;
			}
		} else {
			StepStatus = step_send_data_1;
			break;
		}
		break;

	case step_send_data_1:
		Send_data_to_transparan("Cavli Wireless Message 1\r\n");
		HAL_Delay(1000);
		StepStatus = step_exit_transparan_mode;
		break;

	case step_exit_transparan_mode:
		if (Exit_transparan_mode() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_repeate_enter_transparan_mode;
			HAL_Delay(2000);
			break;
		}
		break;

	case step_repeate_enter_transparan_mode:
		if (Repeate_enter_transparan_mode() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_send_data_2;
			break;
		}
		break;

	case step_send_data_2:
		Send_data_to_transparan("Cavli Wireless Message 2\r\n");
		HAL_Delay(1000);
		StepStatus = step_exit_transparan_mode_2;
		break;

	case step_exit_transparan_mode_2:
		if (Exit_transparan_mode() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			HAL_Delay(1000);
			StepStatus = step_close_TCP;
			break;
		}
		break;

	case step_close_TCP:
		if (TCP_close() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_close_all_TCP;
			break;
		}
		break;

	case step_close_all_TCP:
		if (All_TCP_close() == ERROR) {
			StepStatus = step_pwrkey;
			break;
		} else {
			StepStatus = step_idle;
			break;
		}
		break;

	case step_idle:
		StepStatus = step_idle;
		break;

	default:
		break;
	}

}
