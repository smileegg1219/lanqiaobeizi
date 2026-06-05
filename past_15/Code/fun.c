#include "fun.h"

void led_show(uint8_t led, uint8_t mode)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	
	if(mode)
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8<<(led-1), GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8<<(led-1), GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}
//
int16_t free_A, free_B;
uint16_t captuer_A, captuer_B;
uint8_t free_flag;
uint8_t data_flag_A,data_flag_B;
uint8_t NULL_A, NULL_B;
uint8_t NDA, NDB, NHA, NHB;
uint8_t NHA_flag = 1;
uint8_t NHB_flag = 1;

uint8_t lcd_mode;
uint8_t para_indax;
uint16_t para_PD = 1000;
uint16_t para_PH = 5000;
int16_t para_PX;

uint16_t count_3s,count_1s;
int16_t free_A_max, free_B_max;
int16_t free_A_min = 20000;
int16_t free_B_min = 20000;
uint8_t PD_flag_A,PD_flag_B;
uint8_t long_flag;
//
uint8_t led_flag;
uint8_t B1_data,B2_data,B3_data,B4_data,B1_last,B2_last,B3_last,B4_last;
void key_scan(void)
{
	
	B1_data = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);B2_data = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
	B3_data = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);B4_data = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
	
	if(!B1_data&B1_last) // B1
	{
		if(para_indax == 0)
		{
			para_PD+=100;
			if(para_PD>1000) para_PD = 100;
		}
		else if(para_indax == 1)
		{
			para_PH+=100;
			if(para_PH>10000) para_PH = 1000;
		}
		else
		{
			para_PX+=100;
			if(para_PX>1000) para_PX = -1000;
		}
	}
	if(!B2_data&B2_last) // B2
	{
		if(para_indax == 0)
		{
			para_PD-=100;
			if(para_PD<100) para_PD = 1000;
		}
		else if(para_indax == 1)
		{
			para_PH-=100;
			if(para_PH<1000) para_PH = 10000;
		}
		else
		{
			para_PX-=100;
			if(para_PX<-1000) para_PX = 1000;
		}
	}
	if(lcd_mode != 2)
	{
		if(!B3_data&B3_last) // B3
		{
			if(lcd_mode == 1)
			{
				if(++para_indax>2) para_indax = 0;
			}
			else if(lcd_mode == 0)
			{
				free_flag ^= 1;
			}
		}
	}
	else // ³¤°´
	{
		if(!B3_data&!B3_last)
		{
			long_flag = 1;
			if(count_1s>1000)
			{
				count_1s = 0;
				NDA = 0;NDB = 0;NHA = 0;NHB = 0;
			}
		}
		else if(B3_data&!B3_last)
		{
			long_flag = 0;
			count_1s = 0;
		}
	}
	if(!B4_data&B4_last) // B4
	{
		if(++lcd_mode>2)
		{
			lcd_mode = 0;
			para_indax = 0;
			free_flag = 0;
		}
	}
	
	B1_last = B1_data;B2_last = B2_data;B3_last = B3_data;B4_last = B4_data;
}
char text[20];
void lcd_show(void)
{
	uint16_t temp = GPIOC -> ODR;
	
	if(lcd_mode == 0)
	{
		sprintf(text, "        DATA        ");
		LCD_DisplayStringLine(Line1, (uint8_t *)text);
		if(!free_flag)
		{
			if(!NULL_A)
			{
				if(!data_flag_A) sprintf(text, "     A=%dHz         ", free_A);
				else sprintf(text, "     A=%.2fKHz        ", (float)free_A/1000);
			}
			else sprintf(text, "     A=NULL         ");
			LCD_DisplayStringLine(Line3, (uint8_t *)text);
			if(!NULL_B)
			{
				if(!data_flag_B) sprintf(text, "     B=%dHz         ", free_B);
				else sprintf(text, "     B=%.2fKHz        ", (float)free_B/1000);
			}
			else sprintf(text, "     B=NULL         ");
			LCD_DisplayStringLine(Line4, (uint8_t *)text);
		}
		else
		{
			if(!NULL_A)
			{
				if(!data_flag_A) sprintf(text, "     A=%duS         ", captuer_A);
				else sprintf(text, "     A=%.2fmS         ", (float)captuer_A/1000);
			}
			else sprintf(text, "     A=NULL         ");
			LCD_DisplayStringLine(Line3, (uint8_t *)text);
			if(!NULL_B)
			{
				if(!data_flag_B) sprintf(text, "     B=%duS         ", captuer_B);
				else sprintf(text, "     B=%.2fmS         ", (float)captuer_B/1000);
			}
			else sprintf(text, "     B=NULL         ");
			LCD_DisplayStringLine(Line4, (uint8_t *)text);
		}
		LCD_ClearLine(Line5);LCD_ClearLine(Line6);
	}
	else if(lcd_mode == 1)
	{
		sprintf(text, "        PARA        ");
		LCD_DisplayStringLine(Line1, (uint8_t *)text);
		sprintf(text, "     PD=%dHz        ", para_PD);
		LCD_DisplayStringLine(Line3, (uint8_t *)text);
		sprintf(text, "     PH=%dHz        ", para_PH);
		LCD_DisplayStringLine(Line4, (uint8_t *)text);
		sprintf(text, "     PX=%dHz        ", para_PX);
		LCD_DisplayStringLine(Line5, (uint8_t *)text);
	}
	else
	{
		sprintf(text, "        RECD        ");
		LCD_DisplayStringLine(Line1, (uint8_t *)text);
		sprintf(text, "     NDA=%d         ", NDA);
		LCD_DisplayStringLine(Line3, (uint8_t *)text);
		sprintf(text, "     NDB=%d         ", NDB);
		LCD_DisplayStringLine(Line4, (uint8_t *)text);
		sprintf(text, "     NHA=%d         ", NHA);
		LCD_DisplayStringLine(Line5, (uint8_t *)text);
		sprintf(text, "     NHB=%d         ", NHB);
		LCD_DisplayStringLine(Line6, (uint8_t *)text);
	}
	
	GPIOC -> ODR = temp;
}
void data_proc(void)
{
	//PX
	if(!free_flag)
	{
		if(free_A>1000) data_flag_A = 1;
		else data_flag_A = 0;
		if(free_B>1000) data_flag_B = 1;
		else data_flag_B = 0;
	}
	else
	{
		if(captuer_A>1000) data_flag_A = 1;
		else data_flag_A = 0;
		if(captuer_B>1000) data_flag_B = 1;
		else data_flag_B = 0;
	}
	//PH
	if(NHA_flag==0)
	{
		if(free_A>para_PH)
		{
			NHA_flag = 1;
			NHA++;
		}
	}
	else
	{
		if(free_A<para_PH) NHA_flag = 0;
	}
	
	if(NHB_flag==0)
	{
		if(free_B>para_PH)
		{
			NHB_flag = 1;
			NHB++;
		}
	}
	else
	{
		if(free_B<para_PH) NHB_flag = 0;
	}
	//PD
	
	if(count_3s>3000)
	{
		count_3s = 0;
		free_A_max = 0;
		free_B_max = 0;
		free_A_min = 20000;
		free_B_min = 20000;
		PD_flag_A = 0;
		PD_flag_B = 0;
	}
	else
	{
		if(free_A>free_A_max) free_A_max = free_A;
		if(free_A<free_A_min) free_A_min = free_A;
		if(free_B>free_B_max) free_B_max = free_B;
		if(free_B<free_B_min) free_B_min = free_B;
		if(!PD_flag_A)
		{
			if((free_A_max-free_A_min)>para_PD)
			{
				NDA++;
				PD_flag_A = 1;
			}
		}
		if(!PD_flag_B)
		{
			if((free_B_max-free_B_min)>para_PD)
			{
				NDB++;
				PD_flag_B = 1;
			}
		}
	}
}
void main_pros(void)
{
	led_show(1,(lcd_mode == 0));
	led_show(2,(free_A>para_PH));
	led_show(3,(free_B>para_PH));
	led_show(8,(NDA>2||NDB>2));
	lcd_show();
	key_scan();
	data_proc();
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM16)
	{
		free_B = 1000000/(TIM16->CCR1+1); 
		TIM16->CNT = 0;
		if(free_B>20000) free_B = 20000;
		else if(free_B<400) free_B = 400;
		free_B += para_PX;
		if(free_B<0) NULL_B = 1;
		else NULL_B = 0;
		captuer_B = 1000000/free_B;
		
	}
	if(htim->Instance == TIM2)
	{
		free_A = 1000000/(TIM2->CCR1+1); 
		TIM2->CNT = 0;
		if(free_A>20000) free_A = 20000;
		else if(free_A<400) free_A = 400;
		free_A += para_PX;
		if(free_A<0) NULL_A = 1;
		else NULL_A = 0;
		captuer_A = 1000000/free_A;
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM4)
	{
		count_3s++;
		if(long_flag) count_1s++;
	}
}
