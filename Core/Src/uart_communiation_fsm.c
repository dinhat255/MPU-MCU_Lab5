/*
 * uart_communiation_fsm.c
 *
 *  Created on: Nov 28, 2025
 *      Author: dinhat255
 */

#include "uart_communiation_fsm.h"
#include <stdio.h>
#include <string.h>

char str_tx[50];
uint32_t last_adc_value = 0;
uint32_t timer_start_time = 0;

enum UART_STATE { UART_IDLE, UART_WAIT_OK };
enum UART_STATE uart_state = UART_IDLE;

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;

void uart_communiation_fsm() {
  switch (uart_state) {
  case UART_IDLE:
    if (command_flag == 1) {
      command_flag = 0;

      last_adc_value = HAL_ADC_GetValue(&hadc1);
      sprintf(str_tx, "\r\n!ADC=%d#\r\n", (int)last_adc_value);

      HAL_UART_Transmit(&huart2, (uint8_t *)str_tx, strlen(str_tx), 1000);

      timer_start_time = HAL_GetTick();
      uart_state = UART_WAIT_OK;
    }
    break;

  case UART_WAIT_OK:
    if (command_flag == 1) {
      command_flag = 0;
      last_adc_value = HAL_ADC_GetValue(&hadc1);
      sprintf(str_tx, "\r\n!ADC=%d#\r\n", (int)last_adc_value);
      HAL_UART_Transmit(&huart2, (uint8_t *)str_tx, strlen(str_tx), 1000);
      timer_start_time = HAL_GetTick();
    }

    if (command_flag == 2) {
      command_flag = 0;
      uart_state = UART_IDLE;
    }

    if (HAL_GetTick() - timer_start_time > 3000) {
      sprintf(str_tx, "!ADC=%d#\r\n", (int)last_adc_value);
      HAL_UART_Transmit(&huart2, (uint8_t *)str_tx, strlen(str_tx), 1000);
      timer_start_time = HAL_GetTick();
    }
    break;
  }
}
