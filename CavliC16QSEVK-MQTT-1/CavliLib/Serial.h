/*
 * Serial.h
 *
 *  Created on: Jan 26, 2024
 *      Author: hk
 */

#ifndef SERIAL_H_
#define SERIAL_H_
#include "main.h"
#define C16QS_Buf_Size		(2048)

typedef struct {
	uint16_t read_pointer;
	uint16_t write_pointer;
	uint8_t data_buf[C16QS_Buf_Size];

} C16QS_Buf_s;

extern C16QS_Buf_s C16QS_Buf;
extern uint8_t C16QS_rx;

ErrorStatus C16QS_Serial_Queue_is_empty(C16QS_Buf_s *pointer);
ErrorStatus C16QS_Serial_Enqueue_Queue(C16QS_Buf_s *pointer, uint8_t *data);
void C16QS_Serial_Dequeue_Queue(C16QS_Buf_s *pointer);
uint8_t C16QS_Serial_Front_of_Queue(C16QS_Buf_s *pointer);
void C16QS_Serial_Flush_Queue(C16QS_Buf_s *pointer);
uint16_t C16QS_Serial_Get_Item_Count_in_Queue(C16QS_Buf_s *pointer);
char C16QS_Get_Data_Byte(void);

#endif /* SERIAL_H_ */
