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

/* TCP Informations */
#define DestinationIP			"111.222.333.444"
#define DestinationPort			(5555)

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
	step_gpio7_status_control,
	step_gpio7_set_low,
	step_gpio7_set_high,
	step_error,
} Step_e;

extern Step_e StepStatus;

typedef enum {
	LogicLow = 0, LogicHigh = 1, LogicError = 2,
} LogicStatus_e;

void PowerKeyControl(Status_e sts);
void PowerKeyFunction(void);

void RstKeyControl(Status_e sts);
void ResetKeyFunction(void);

void Clear_Buffer(char *buf, uint16_t len);
void Echo_Disable(void);
ErrorStatus Alive_Control(void);
ErrorStatus Control_OK_Message(char *buf);

ErrorStatus CIPSTART_transparan_setting(char *IP, uint16_t Port);
ErrorStatus Control_Data_Enter(char *buf);

ErrorStatus Control_Send_OK(char *buf);

LogicStatus_e GPIO7_Status_Control(void);
LogicStatus_e Control_GPIO7_Status_Message(char *buf);

ErrorStatus GPIO7_Set(LogicStatus_e status);

void C16QS_GPIO7_Sequence(void);

#endif /* C16QS_H_ */
