#include "fun.h"

//变量定义
uint8_t long_flag;
uint32_t count_key;
uint32_t count;

uint8_t long_two;
uint8_t flag_two;
uint32_t count_two;
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
	sprintf(text,"count=%d",count);
	LCD_DisplayStringLine(Line2,(uint8_t*)text);
	GPIOC->ODR=temp;
}
/*按键专题，明确概念
(!B1_state&B1_last)是单次按下
(!B1_state&!B1_last)是长按
(B1_state&!B1_last)是抬起单次 */

void key_scan()  
{
	uint32_t timeTick=HAL_GetTick();
	static uint32_t lastTick;
	static bool B1_state,B2_state,B3_state,B4_state;
	static bool B1_last=1,B2_last=1,B3_last=1,B4_last=1;
	//消抖的作用
//	if (timeTick-lastTick<100) return;
//	lastTick=timeTick;
	
	B1_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	B2_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	B3_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	B4_state=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);

	//第一种，单击+1，长按一直+
	
	if (!B1_state&!B1_last)
	{
		long_flag=1;  //长按标记
		if(count_key>2000) count++;  //长按大于2s则一直+
	}
	 if(B1_state&!B1_last)  //抬起来
	{
		 if (count_key<2000) count++;  //小于2s则是单次+1
		count_key=0;          //标记、计时都清零
		long_flag=0;
	}
	//第二种，单机+1, 长按+2
	if (!long_two){
		if (!B2_state&!B2_last)
	{
		long_flag=1;
		if(count_key>2000)
		{
			count+=2;
			long_two=1;
		}
	}
}
	 if (B2_state&!B2_last)
	{
		if(count_key<2000) count++;
		long_flag=0;
		long_two=0;
		count_key=0;
	}
	//第三种，单击+1，双击+2，多击只要标志位嵌套就行
	if (flag_two==0){
		if (B3_state&!B3_last) 
			flag_two=1;
}
	else if (flag_two){
		if (count_two<300){
			if (B3_state&!B3_last)
			{
				count+=2;
				count_two=0;
				flag_two=0;
			}
		}
		else {
				count++;
				count_two=0;
				flag_two=0;	
		}		
	}
	//第四种，单击+1，双击+2，长按+3
	if (!long_two&&!flag_two)
	{
		 if (!B4_state&!B4_last)
	   {
		 long_flag=1;
		 if(count_key>2000) 
		 { 
			 count+=3;
			long_two=1;
		 }
		 }
	}
	
	
	if(!flag_two)
	{
	if (B4_state&!B4_last)
	{
		if(count_key<2000) 
		 {
			 flag_two=1;
	   }
		 long_two=0;
		 long_flag=0;
     count_key =0;
	 }
  }

	else if (flag_two){
		if (count_two<300){
			if (B4_state&!B4_last){
				count+=2;
				count_two=0;
				flag_two=0;
			}
		}
		else {
				count+=1;
				count_two=0;
				flag_two=0;
			}
	 }
	

   B1_last=B1_state; B2_last=B2_state; B3_last=B3_state;B4_last=B4_state;
	 
}

void loop (void)
{
	  lcd_show();
	  key_scan();
//   	led_show(1,0);
//	led_show(2,1);
}

void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  if (long_flag)   count_key++;
	if (flag_two) count_two++;
  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}