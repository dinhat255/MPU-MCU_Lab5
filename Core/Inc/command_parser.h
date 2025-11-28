/*
 * command_parser.h
 *
 *  Created on: Nov 28, 2025
 *      Author: DUYNHAT
 */

#ifndef INC_COMMAND_PARSER_H_
#define INC_COMMAND_PARSER_H_

#include "main.h"
#include <stdint.h>
#define MAX_BUFFER_SIZE 30

extern uint8_t buffer[];
extern uint8_t buffer_flag;
extern uint8_t temp;
extern uint8_t command_flag;
extern uint8_t index_buffer;
// 1 = RST
// 2 = OK

void command_parser_fsm(void);

#endif /* INC_COMMAND_PARSER_H_ */
