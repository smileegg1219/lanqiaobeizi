#ifndef __FUN_H__
#define __FUN_H__
#include "main.h"
#include "gpio.h"
#include  "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "adc.h"
#include "tim.h"

#include "lcd.h"
#include "i2c_hal.h"

void loop (void);
extern  uint8_t mcp4017_data;
#endif