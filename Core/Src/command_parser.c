/*
 * command_parser.c
 *
 *  Created on: Nov 28, 2025
 *      Author: DUYNHAT
 */
#include "command_parser.h"

uint8_t temp = 0;
volatile uint8_t buffer[MAX_BUFFER_SIZE];
volatile uint8_t buffer_flag = 0;
volatile uint8_t command_flag = 0;
volatile uint8_t index_buffer = 0;

static uint8_t parser_index = 0;

void command_parser_fsm() {
  static int state = 0;

  while (parser_index != index_buffer) {
    uint8_t c = buffer[parser_index];
    parser_index = (parser_index + 1) % MAX_BUFFER_SIZE;

    if (c == '!') {
      state = 1;
      continue;
    }

    switch (state) {
    case 0:
      // if (c == '!') state = 1; // Handled above
      break;

    case 1:
      if (c == 'R')
        state = 2;
      else if (c == 'O')
        state = 10;
      else
        state = 0;
      break;

    case 2:
      if (c == 'S')
        state = 3;
      else
        state = 0;
      break;

    case 3:
      if (c == 'T')
        state = 4;
      else
        state = 0;
      break;

    case 4:
      if (c == '#')
        command_flag = 1; // !RST#
      state = 0;
      break;

    case 10:
      if (c == 'K')
        state = 11;
      else
        state = 0;
      break;

    case 11:
      if (c == '#')
        command_flag = 2; // !OK#
      state = 0;
      break;
    }
  }
}
