/*
 * global.h
 *
 *  Created on: Nov 28, 2025
 *      Author: dinhat255
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"

#define MAX_BUFFER_SIZE 30

extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;

extern uint8_t read_index;
extern uint8_t command_flag;

#endif /* INC_GLOBAL_H_ */
