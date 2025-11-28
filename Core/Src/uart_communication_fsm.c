/*
 * uart_communication_fsm.c
 *
 *  Created on: Nov 28, 2025
 *      Author: DUYNHAT
 */
#include "uart_communication_fsm.h"


extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;

void uart_communication_fsm()
{
    static int state = 0;
    static uint32_t timestamp = 0;
    static char msg[50];
    uint32_t adc;

    switch(state)
    {
    case 0:
        if (command_flag == 1) // !RST#
        {
            command_flag = 0;

            HAL_ADC_Start(&hadc1);
            adc = HAL_ADC_GetValue(&hadc1);

            sprintf(msg, "!ADC=%lu#", adc);
            HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 1000);

            timestamp = HAL_GetTick();
            state = 1;
        }
        break;

    case 1:
        if (command_flag == 2) // !OK#
        {
            command_flag = 0;
            state = 0;
        }
        else if (HAL_GetTick() - timestamp >= 3000)
        {
            HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 1000);
            timestamp = HAL_GetTick();
        }
        break;
    }
}



