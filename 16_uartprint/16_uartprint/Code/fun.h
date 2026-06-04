#ifndef __FUN_H__
#define __FUN_H__

#define RX_MUX 30

#include "main.h"
#include "gpio.h"
#include  "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "dma.h"

#include "lcd.h"
#include "i2c_hal.h"

void loop (void);
extern char uart_rx[RX_MUX];
extern bool rx_flag;
#endif