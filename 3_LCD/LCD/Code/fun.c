#include "fun.h"

void led_show(uint8_t wela,bool state)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	if (state) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(wela-1),GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(wela-1),GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

void lcd_show()
{
	uint16_t temp=GPIOC->ODR;
	char text[20]={'\0'};
	sprintf(text,"    c    Orin");
	LCD_DisplayStringLine(Line2,(uint8_t*)text);
	GPIOC->ODR=temp;
}
void loop (void)
{
	  lcd_show();
   	led_show(1,0);
//	led_show(2,1);
}
