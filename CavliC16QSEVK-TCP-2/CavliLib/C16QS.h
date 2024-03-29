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
	step_func_control,
	step_func_enable,
	step_NetworkReg,
	step_8,
	step_internet_control,
	step_cipmux_setting,
	step_transparan_setting,
	step_control_transparan_setting,
	step_start_transparan_connection,
	step_send_data_1,
	step_exit_transparan_mode,
	step_repeate_enter_transparan_mode,
	step_send_data_2,
	step_exit_transparan_mode_2,
	step_close_TCP,
	step_close_all_TCP,
	step_idle
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

ErrorStatus CIPSTART_transparan_setting(char *IP, uint16_t Port);
ErrorStatus Control_Data_Enter(char* buf);

ErrorStatus Modul_Function_Enable(void);

ErrorStatus Network_Register_Control(void);
ErrorStatus Control_NetworReg_Message(char *buf);

ErrorStatus Internet_Control(void);
ErrorStatus Control_Internet_Message(char *buf);

ErrorStatus CIPMUX_setting(void);
ErrorStatus Control_CIPMUX_OK_Message(char *buf);

ErrorStatus Transparan_setting(void);
ErrorStatus Control_Transparan_OK_Message(char *buf);

ErrorStatus Control_transparan_setting(void);
ErrorStatus Control_transparan_setting_Message(char *buf);

ErrorStatus CIPSTART_setting(char* IP, uint16_t Port);
ErrorStatus Control_CIPSTART_response(char *buf);

ErrorStatus Control_Send_OK(char* buf);

void Send_data_to_transparan(char *data);

ErrorStatus Exit_transparan_mode(void);
ErrorStatus Control_Exit_Message(char *buf);

ErrorStatus Repeate_enter_transparan_mode(void);

ErrorStatus TCP_close(void);
ErrorStatus Control_TCP_Close_Message(char *buf);

ErrorStatus All_TCP_close(void);
ErrorStatus Control_All_TCP_Close_Message(char *buf);


void C16QS_TCP_Sequence(void);

#endif /* C16QS_H_ */
