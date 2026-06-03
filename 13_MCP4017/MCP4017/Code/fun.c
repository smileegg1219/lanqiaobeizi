#include "fun.h"
/*--------------变量定义-----------------*/
	char text[20]={'\0'};
	uint8_t mcp4017_data=0;
	double msp_adc;
/*--------------led显示-----------------*/
void led_show(uint8_t wela,bool state)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	if (state) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(wela-1),GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(wela-1),GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}
/*--------------屏幕-----------------*/
void lcd_show()
{
	uint16_t temp=GPIOC->ODR;

	sprintf(text,"       Orin");
	LCD_DisplayStringLine(Line2,(uint8_t*)text);
	sprintf(text,"    %u    ",mcp4017_data);
	LCD_DisplayStringLine(Line3,(uint8_t*)text);
		sprintf(text,"    %.2f    ",msp_adc);
	LCD_DisplayStringLine(Line4,(uint8_t*)text);
	GPIOC->ODR=temp;
}
/*--------------按键-----------------*/
void key_scan()
{
	uint32_t timeTick=HAL_GetTick();
	static uint32_t lastTick;
	static bool B1_state,B2_state,B3_state,B4_state;
	static bool B1_last,B2_last,B3_last,B4_last;
	if (timeTick-lastTick<200) return;
	lastTick=timeTick;
	B1_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	B2_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	B3_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2); 
	B4_state=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);

	if (!B1_state&B1_last)
	{
		led_show(1,1);
	}
		if (!B2_state&B2_last)
	{
		led_show(1,0);
	}
		if (!B3_state&B3_last)
	{
		led_show(2,1);
	}
		if (!B4_state&B4_last)
	{
		led_show(2,0);
	}
   B1_last=B1_state; B2_last=B2_state; B3_last=B3_state;B4_last=B4_state;
	 
}
/*--------------可编程电阻的adc-----------------*/
double adc_read(ADC_HandleTypeDef *hadc)
{
	HAL_ADC_Start(hadc);
	uint16_t adc_value= HAL_ADC_GetValue(hadc);
	
	return adc_value*3.3/4095.0;
}
/*--------------循环函数-----------------*/
void loop (void)
{
	  lcd_show();
	  key_scan();
		mcp4017_write(13);  //mcp的范围是7位，也就是0~127,电阻最大是100K
	  mcp4017_data=mcp4017_read();
	  msp_adc=adc_read(&hadc1);
//   	led_show(1,0);
//	led_show(2,1);
}
