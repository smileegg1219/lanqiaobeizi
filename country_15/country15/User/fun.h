#ifndef __FUN_H__
#define __FUN_H__

#include "main.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"

#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "adc.h"


#include "lcd.h"
#include "i2c_hal.h"


extern char uart_rx[30];
extern uint8_t uart_flag;
void main_proc(void);

#endif