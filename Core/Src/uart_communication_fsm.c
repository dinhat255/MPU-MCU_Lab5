/*
 * uart_communication_fsm.c
 *
 *  Created on: Nov 28, 2025
 *      Author: DUYNHAT
 */
#include "uart_communication_fsm.h"

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;
extern volatile uint8_t command_flag;

void uart_communication_fsm() {
  static int state = 0;
  static int counter = 0;
  static char msg[50];
  uint32_t adc;

  // Ưu tiên xử lý lệnh !OK# để dừng ngay lập tức bất kể trạng thái
  if (command_flag == 2) { // !OK#
    command_flag = 0;
    state = 0;
    counter = 0;
    return;
  }

  switch (state) {

  //-------------------------------
  // STATE 0 — CHỜ !RST#
  //-------------------------------
  case 0:
    if (command_flag == 1) { // !RST#
      // Chỉ gửi khi UART sẵn sàng
      if (HAL_UART_GetState(&huart2) == HAL_UART_STATE_READY) {
        adc = HAL_ADC_GetValue(&hadc1);
        sprintf(msg, "!ADC=%lu#\r\n", adc);
        HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, strlen(msg));

        command_flag = 0;
        counter = 0;
        state = 1;
      }
      // Nếu UART bận, giữ command_flag = 1 để thử lại ở vòng lặp sau
    }
    break;

  //-------------------------------
  // STATE 1 — ĐÃ GỬI ADC, CHỜ !OK#
  //-------------------------------
  case 1:
    if (command_flag == 1) { // !RST# AGAIN -> Gửi lại ngay
      if (HAL_UART_GetState(&huart2) == HAL_UART_STATE_READY) {
        adc = HAL_ADC_GetValue(&hadc1);
        sprintf(msg, "!ADC=%lu#\r\n", adc);
        HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, strlen(msg));

        command_flag = 0;
        counter = 0;
      }
    } else {
      counter++;
      if (counter >= 300) { // 3 giây
        if (HAL_UART_GetState(&huart2) == HAL_UART_STATE_READY) {
          adc = HAL_ADC_GetValue(&hadc1);
          sprintf(msg, "!ADC=%lu#\r\n", adc);
          HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, strlen(msg));
          counter = 0;
        }
      }
    }
    break;
  }
}
