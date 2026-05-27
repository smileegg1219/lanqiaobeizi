#ifndef __FUN_H__
#define __FUN_H__
#include "main.h"
#include "gpio.h"
#include "dma.h"
#include "usart.h"
#include "tim.h"

#include  "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"

#include "lcd.h"
void loop (void);

extern  volatile bool uart_flag;
extern   uint16_t str_length;
extern  uint8_t uart_rx_str[30];
extern  uint8_t uart_tx_str[30];


#endif