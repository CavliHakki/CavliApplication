/*
 * Serial.c
 *
 *  Created on: Jan 26, 2024
 *      Author: hk
 */
#include "Serial.h"

C16QS_Buf_s C16QS_Buf;
uint8_t C16QS_rx = 0;

ErrorStatus C16QS_Serial_Queue_is_empty(C16QS_Buf_s *pointer) {
	if (pointer->read_pointer == pointer->write_pointer) {
		pointer->read_pointer = 0;
		pointer->write_pointer = 0;
		return SUCCESS;
	}
	return ERROR;
}

ErrorStatus C16QS_Serial_Enqueue_Queue(C16QS_Buf_s *pointer, uint8_t *data) {
	pointer->data_buf[pointer->write_pointer] = *data;
	pointer->write_pointer = (pointer->write_pointer + 1) % C16QS_Buf_Size;
	return SUCCESS;
}

void C16QS_Serial_Dequeue_Queue(C16QS_Buf_s *pointer) {
	if (pointer->read_pointer == pointer->write_pointer) {
		pointer->read_pointer = 0;
		pointer->write_pointer = 0;
	} else {
		pointer->read_pointer = (pointer->read_pointer + 1) % C16QS_Buf_Size;
	}
}

uint8_t C16QS_Serial_Front_of_Queue(C16QS_Buf_s *pointer) {
	uint8_t r_byte = 0;
	r_byte = pointer->data_buf[pointer->read_pointer];
	return r_byte;
}

void C16QS_Serial_Flush_Queue(C16QS_Buf_s *pointer) {
	pointer->read_pointer = 0;
	pointer->write_pointer = 0;
}

uint16_t C16QS_Serial_Get_Item_Count_in_Queue(C16QS_Buf_s *pointer) {
	uint16_t r_count = 0;
	r_count = ((pointer->write_pointer + C16QS_Buf_Size)
			- (pointer->read_pointer)) % C16QS_Buf_Size;
	return r_count;
}

char C16QS_Get_Data_Byte(void) {
	uint8_t d_byte = C16QS_Serial_Front_of_Queue(&C16QS_Buf);
	C16QS_Serial_Dequeue_Queue(&C16QS_Buf);
	return d_byte;
}
