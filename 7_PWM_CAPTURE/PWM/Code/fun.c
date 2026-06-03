#include "fun.h"

void led_show(uint8_t wela,bool state)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	if (state) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(wela-1),GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(wela-1),GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}
/*--------------±äÁ¿ÉùÃ÷---------------------*/
uint32_t R39_fre;

volatile bool  uart_flag;
uint16_t str_length;
uint8_t uart_rx_str[30]={'\0'};
uint8_t uart_tx_str[30]={'\0'};

void uart_send(char*str)
{
	uint16_t len=strlen(str);
	HAL_UART_Transmit_DMA(&huart1,(uint8_t*)str,len);
}
void lcd_show()
{
	uint16_t temp=GPIOC->ODR;
	char text[20]={'\0'};
	sprintf(text,"handsome wy");
	LCD_DisplayStringLine(Line1,(uint8_t*)text);
	
	sprintf(text,"R39:%d",R39_fre);
	LCD_DisplayStringLine(Line3,(uint8_t*)text);
	GPIOC->ODR=temp;
}



void uart_proc()
{
	if(uart_flag)
	{
	if (!strcmp((char*)uart_rx_str,"Orion")) 
		uart_send("bighandsome");
	else 
		uart_send("error");
	  uart_flag=0;
	  memset(uart_rx_str,'\0',str_length);
   }
}

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

void loop (void)
{
	  lcd_show();
	 //key_scan();
	  uart_proc();

}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	if (huart->Instance==USART1)
	{
		uart_flag=1;
		str_length=Size;
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1,uart_rx_str,30);
	}
	
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM16)
	{
		R39_fre=HZ_1MHz/(TIM16->CCR1+1);
		TIM16->CNT=0;
	}
}
