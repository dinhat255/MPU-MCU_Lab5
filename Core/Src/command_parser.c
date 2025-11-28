/*
 * command_parser.c
 *
 *  Created on: Nov 28, 2025
 *      Author: dinhat255
 */

#include "command_parser.h"
#include <string.h>

void command_parser_fsm() {
  static char cmd_buffer[MAX_BUFFER_SIZE];
  static uint8_t cmd_index = 0;
  static enum PARSER_STATE {
    PARSER_IDLE,
    PARSER_RECEIVING
  } parser_state = PARSER_IDLE;

  while (read_index != index_buffer) {
    uint8_t char_rx = buffer[read_index++];
    if (read_index == MAX_BUFFER_SIZE)
      read_index = 0;

    if (char_rx == 8 || char_rx == 127) {
      if (parser_state == PARSER_RECEIVING && cmd_index > 0) {
        cmd_index--;
      }
      continue;
    }

    if (char_rx == '!') {
      cmd_index = 0;
      cmd_buffer[cmd_index++] = char_rx;
      parser_state = PARSER_RECEIVING;
      continue;
    }

    switch (parser_state) {
    case PARSER_IDLE:
      break;

    case PARSER_RECEIVING:
      cmd_buffer[cmd_index++] = char_rx;

      if (char_rx == '#') {
        cmd_buffer[cmd_index] = '\0';
        if (strcmp(cmd_buffer, "!RST#") == 0) {
          command_flag = 1;
          parser_state = PARSER_IDLE;
        } else if (strcmp(cmd_buffer, "!OK#") == 0) {
          command_flag = 2;
          parser_state = PARSER_IDLE;
        }
      } else if (cmd_index >= MAX_BUFFER_SIZE - 1) {
        parser_state = PARSER_IDLE;
        cmd_index = 0;
      }
      break;
    }
  }
}
