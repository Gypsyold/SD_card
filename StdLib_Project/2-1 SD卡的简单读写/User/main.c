#include "stm32f4xx.h"                  // Device header
#include "board.h"
#include "bsp_uart.h"
#include "stdio.h"
#include "bsp_key.h"
#include "sys_timer.h"
#include "sdio_test.h"



int main(void)
{

	
	board_init();
	uart1_init(115200);
	sys_Timer_Init();
	bsp_key_init();
	printf("start_02_demo\r\n");

	while(1)
	{
		KEYS waitKey = Key_GetNum();
		if(waitKey == KEY_UP)
		{
			printf("\r\n开始进行SD卡读写实验\r\n");	
			SD_Test();			
		}	
		
		
		
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{

		Ket_Tick();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
