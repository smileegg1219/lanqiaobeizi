#include "fun.h"

void led_show(uint8_t wela,bool state)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	if (state) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(wela-1),GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(wela-1),GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;
	
void lcd_show()
{
	uint16_t temp=GPIOC->ODR;
	char text[20]={'\0'};
	sprintf(text,"Time:%02d-%02d-%02d",time.Hours,time.Minutes,time.Seconds);
	LCD_DisplayStringLine(Line1,(uint8_t*)text);
	sprintf(text,"Date:%02d-%02d-%02d",date.Year+2000,date.Month,date.Date);
	LCD_DisplayStringLine(Line2,(uint8_t*)text);
	GPIOC->ODR=temp;
}

//设置时间
void set_up()
{
	RTC_TimeTypeDef time_set={23,30,12};
	RTC_DateTypeDef date_set={5,4,10,26};  //注意年份数值是0~99，2026要用26+2000
	HAL_RTC_SetTime(&hrtc,&time_set,RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc,&date_set,RTC_FORMAT_BIN);
	
}
bool led_flag;
void loop (void)
{
		 //在hal库中，时间和日历必须一起读取才有用
	  HAL_RTC_GetTime(&hrtc,&time,RTC_FORMAT_BIN);
	  HAL_RTC_GetDate(&hrtc,&date,RTC_FORMAT_BIN);
	  lcd_show();
   	led_show(1,led_flag);
    
//	led_show(2,1);
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	led_flag=1;
}