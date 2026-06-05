#include "fun.h"
/*----------------------------------变量定义----------------------------------*/
char text[20]={'\0'};
char uart_rx[30]={'\0'};
uint8_t uart_flag;
uint8_t B1_state,B2_state,B3_state,B4_state,B1_last,B2_last,B3_last,B4_last;
uint32_t  fre_R40;
double volt_R37;
uint8_t lcd_mode;
uint8_t ST_state;
double CP_X,CP_Y;
uint16_t TP_X,TP_Y;
double SE_speed;
uint16_t RN_quantity;
double  R_parameter=1.0;
uint8_t B_parameter=10;
double TS_path;
uint32_t TT_time;
uint8_t parameter_index;//参数界面：参数选择标志
uint8_t time2s_flag;
uint32_t time2s;
uint8_t free_target;
/*----------------------------------LED灯----------------------------------*/
void led_show(uint8_t led,bool state){
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,1);
	if (state) 
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(led-1),0);
	else 
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(led-1),1);	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,0);
}
/*----------------------------------LCD----------------------------------*/
void lcd_show(void)
{
	uint16_t temp =GPIOC->ODR;
	if (lcd_mode==0)
	{
	sprintf(text,"        DATA        ");
	LCD_DisplayStringLine(Line1, (u8 *)text);
	if (ST_state==0) sprintf(text,"     ST=Idle        ");
	if (ST_state==1) sprintf(text,"     ST=Busy        ");
	if (ST_state==2) sprintf(text,"     ST=Wait        ");
	LCD_DisplayStringLine(Line3, (u8 *)text);
	sprintf(text,"     CP=%d,%d       ",(uint16_t)CP_X,(uint16_t)CP_Y);
	LCD_DisplayStringLine(Line4, (u8 *)text);
	if (ST_state==0) sprintf(text,"     TP=NF        ");
		else sprintf(text,"     TP=%d,%d       ",TP_X,TP_Y);
	LCD_DisplayStringLine(Line5, (u8 *)text);
	sprintf(text,"     SE=%.1f     ",SE_speed);
	LCD_DisplayStringLine(Line6, (u8 *)text);
	sprintf(text,"     RN=%d       ",RN_quantity);
	LCD_DisplayStringLine(Line7, (u8 *)text);
	}
		if (lcd_mode==1)
	{
	sprintf(text,"        PARA        ");
	LCD_DisplayStringLine(Line1, (u8 *)text);
	sprintf(text,"     R=%.1f         ",R_parameter);
	LCD_DisplayStringLine(Line3, (u8 *)text);
	sprintf(text,"     B=%d        ",B_parameter);
	LCD_DisplayStringLine(Line4, (u8 *)text);
  LCD_ClearLine(Line5);LCD_ClearLine(Line6);LCD_ClearLine(Line7);

	}
		if (lcd_mode==2)
	{
	sprintf(text,"        RECD        ");
	LCD_DisplayStringLine(Line1, (u8 *)text);
	sprintf(text,"     TS=%.1f        ",TS_path);
	LCD_DisplayStringLine(Line3, (u8 *)text);
	sprintf(text,"     TT=%d        ",TT_time/1000);
	LCD_DisplayStringLine(Line4, (u8 *)text);
	}
	GPIOC->ODR=temp;

}
/*----------------------------------按键----------------------------------*/
void key_scan(void)
{
	B1_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	B2_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	B3_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	B4_state=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
	
	if (!B1_state&&B1_last){
		if (free_target){
			if (++ST_state>2) ST_state=1;
		}
		
	}
	if (!B2_state&&B2_last){
		if (lcd_mode==0) parameter_index=0; //保证进入界面2时候是参数R；
		if (++lcd_mode>2) lcd_mode=0;
	}
	//参数界面
	if (lcd_mode==1){
	if (!B3_state&&B3_last){
		parameter_index^=1;
	}
	if (!B4_state&&B4_last){
		if (parameter_index){
			B_parameter+=10;
			if (B_parameter>100) B_parameter=10;
		}
		else {
			R_parameter+=0.1;
			if (R_parameter>=2.1) R_parameter=1.0;
		}
	 }
  }
	//运行界面按键
  if (lcd_mode==0&&ST_state==0){
	  if (!B3_state&&!B3_last&&!B4_state&&!B4_last){
			time2s_flag=1;
			if (time2s>=2000 ){
				time2s_flag=0;
				time2s=0;
				TS_path=0;
				TT_time=0;
			}
		}
		 if ((B3_state&&!B3_last)||(B4_state&&!B4_last)){
			 	time2s_flag=0;
				time2s=0;
		}
		
	}

	B1_last=B1_state;
	B2_last=B2_state;
	B3_last=B3_state;
	B4_last=B4_state;
	
}
/*----------------------------------ADC读取----------------------------------*/
double adc_read(ADC_HandleTypeDef *hadc){
	 HAL_ADC_Start(hadc);
	 uint16_t adc_value=HAL_ADC_GetValue(hadc);
	return adc_value*3.3/4095;
}
/*----------------------------------数据处理----------------------------------*/
void data_proc(void){
	volt_R37=adc_read(&hadc2);
	if (uart_flag)
	{
		uint16_t uart_length =strlen(uart_rx);
		
		
	}
	//速度值
	if (ST_state == 1){
		SE_speed=3.14*R_parameter*fre_R40/100+B_parameter;
		
	}
	uart_flag=0;
	memset(uart_rx,'\0',sizeof uart_rx);
	
}
/*----------------------------------循环主函数----------------------------------*/
void main_proc(void)
{
	
	lcd_show();
	key_scan();
	data_proc();
}




/*----------------------------------回调函数----------------------------------*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM2){
		fre_R40=1000000/(TIM2->CCR1+1);
		TIM2->CNT=0;
		if (fre_R40<400) fre_R40=400;
		else if (fre_R40>20000) fre_R40=20000;
	}
	
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if (huart->Instance == USART1){
		uart_flag=1;
		HAL_UARTEx_ReceiveToIdle_DMA(huart, (uint8_t *)uart_rx, 100);
	}
}

int fputc (int ch,FILE *f){
	HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,50);
	return ch;
}


void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
 if (time2s_flag) time2s++;
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}
