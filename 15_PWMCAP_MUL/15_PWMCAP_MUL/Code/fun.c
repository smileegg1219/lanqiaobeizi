#include "fun.h"
/*--------------变量定义-----------------*/
	char text[20];
	uint8_t mcp4017_data=0;
  double mcp_adc=0;
  double R38_adc=0;
	uint16_t R39_fre;
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
	sprintf(text,"    MCP=%u    ",mcp4017_data);
	LCD_DisplayStringLine(Line3,(uint8_t*)text);
	sprintf(text,"    mcp= %.2f ",mcp_adc);
	LCD_DisplayStringLine(Line4,(uint8_t*)text);
	sprintf(text,"    R38= %.2f ",R38_adc);
	LCD_DisplayStringLine(Line5,(uint8_t*)text);
	sprintf(text,"    R39= %dHz ",R39_fre);
	LCD_DisplayStringLine(Line6,(uint8_t*)text);
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
void adc_read(double *adc_volt1,double *adc_volt2)
{
	HAL_ADC_Start(&hadc1);
	uint16_t mcp_value= HAL_ADC_GetValue(&hadc1);
	*adc_volt1=mcp_value*3.3/4095.0;
	
	HAL_ADC_Start(&hadc1);
	uint16_t R38_value= HAL_ADC_GetValue(&hadc1);
	*adc_volt2=R38_value*3.3/4095.0;
	
}
/*--------------循环函数-----------------*/
void loop (void)
{
	  
	  key_scan();
		lcd_show();
		mcp4017_write(13);  //mcp的范围是7位，也就是0~127,电阻最大是100K
	  mcp4017_data=mcp4017_read();
   	adc_read(&mcp_adc,&R38_adc);

//   	led_show(1,0);
//	led_show(2,1);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if (htim ->Instance==TIM3){
		R39_fre=1000000/(TIM3->CCR1+1);
		TIM3->CNT=0;
	}
	
}
