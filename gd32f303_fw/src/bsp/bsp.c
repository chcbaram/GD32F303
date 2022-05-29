/*
 * bsp.c
 *
 *  Created on: 2022. 5. 28.
 *      Author: Baram
 */


#include "bsp.h"


static volatile uint32_t systick_counter = 0;



void SysTick_Handler(void)
{
  systick_counter++;
}





bool bspInit(void)
{
  SysTick_Config(SystemCoreClock / 1000U);
  NVIC_SetPriority(SysTick_IRQn, 0x00U);


  rcu_periph_clock_enable(RCU_GPIOB);

  return true;
}

void delay(uint32_t ms)
{
#ifdef _USE_HW_RTOS
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
    osDelay(ms);
  }
  else
  {
    HAL_Delay(ms);
  }
#else
  uint32_t pre_time = systick_counter;

  while(systick_counter-pre_time < ms);
#endif
}

uint32_t millis(void)
{
  return systick_counter;
}

int __io_putchar(int ch)
{
  //uartWrite(_DEF_UART1, (uint8_t *)&ch, 1);
  return 1;
}
