#include "fun.h"

//led的控制函数
void led_show(uint8_t wela,bool state)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	if (state) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(wela-1),GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8<<(wela-1),GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

//发送函数，在前面先定义
void uart_send(char*str)
{
	uint16_t len=strlen(str);
	HAL_UART_Transmit_DMA(&huart1,(uint8_t*)str,len);
}

//lcd屏幕显示函数
void lcd_show()
{
	uint16_t temp=GPIOC->ODR;
	char text[20]={'\0'};
	sprintf(text,"    c    Orin");
	LCD_DisplayStringLine(Line2,(uint8_t*)text);
	GPIOC->ODR=temp;
}
//串口使用变量
 volatile bool  uart_flag;
 uint16_t str_length;
 uint8_t uart_rx_str[30]={'\0'};
uint8_t uart_tx_str[30]={'\0'};
//串口处理函数
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
//按键
void key_scan()
{
	uint32_t timeTick=HAL_GetTick();
	static uint32_t lastTick;
	static bool B1_state,B2_state,B3_state,B4_state;
	static bool B1_last,B2_last,B3_last,B4_last;
	//消抖
	if (timeTick-lastTick<200) return;
	lastTick=timeTick;
	//单次检测，防止长按
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
//led反转闪烁的标志位
bool led_flag;
bool led_flag2;
uint32_t time_led;
void loop (void)  //LED4 500ms  LED5 1s
{
	  lcd_show();
	 //key_scan();
	  uart_proc();
	 
	//uart_send("bighandsome");
    led_show(4,led_flag);
	
   //使用滴答计时器
	if(uwTick-time_led>1000)
	{
		led_flag2^=1;
		time_led=uwTick;
	}
	
	led_show(5,led_flag2);

}

//串口空闲接受中断dma
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	if (huart->Instance==USART1)
	{
		led_show(2,1);
		uart_flag=1;
		str_length=Size;
	}
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,uart_rx_str,30);
}
//定时器中断回调
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)  //本工程配置的是2Hz频率
{
	if (htim->Instance ==TIM4)
		led_flag^=1;
}


//// 这是 HAL 库专用的错误处理回调函数
//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
//{
//	if (huart->Instance == USART1)
//	{
//		// 如果进到了这里，说明硬件报错了！
//		// 我们点亮 LED 8 来作为报警灯
//		led_show(5, 1); 
//	
//		// 强行清除溢出标志，把状态机救回来，防止死锁
//		__HAL_UART_CLEAR_OREFLAG(huart);
//		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart_rx_str, 30);
//	}
//}



