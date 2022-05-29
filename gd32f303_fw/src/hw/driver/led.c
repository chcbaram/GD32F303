/*
 * led.c
 *
 *  Created on: 2022. 5. 29.
 *      Author: Baram
 */


#include "led.h"
#ifdef _USE_HW_CLI
#include "cli.h"
#endif


#ifdef _USE_HW_LED

typedef struct
{
  uint32_t port;
  uint32_t pin;
  bit_status on_state;
  bit_status off_state;
} led_tbl_t;


const led_tbl_t led_tbl[LED_MAX_CH] =
    {
        {GPIOB, GPIO_PIN_2, SET, RESET},
    };


#ifdef _USE_HW_CLI
static void cliLed(cli_args_t *args);
#endif


bool ledInit(void)
{
  bool ret = true;



  for (int i=0; i<LED_MAX_CH; i++)
  {
    gpio_init(led_tbl[i].port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, led_tbl[i].pin);

    ledOff(i);
  }

#ifdef _USE_HW_CLI
  cliAdd("led", cliLed);
#endif

  return ret;
}

void ledOn(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  gpio_bit_write(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].on_state);
}

void ledOff(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  gpio_bit_write(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].off_state);
}

void ledToggle(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;


  gpio_bit_write(led_tbl[ch].port,
                 led_tbl[ch].pin,
                 !gpio_output_bit_get(led_tbl[ch].port, led_tbl[ch].pin));

}





#ifdef _USE_HW_CLI

void cliLed(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 3 && args->isStr(0, "toggle") == true)
  {
    uint8_t  led_ch;
    uint32_t toggle_time;
    uint32_t pre_time;

    led_ch      = (uint8_t)args->getData(1);
    toggle_time = (uint32_t)args->getData(2);

    if (led_ch > 0)
    {
      led_ch--;
    }

    pre_time = millis();
    while(cliKeepLoop())
    {
      if (millis()-pre_time >= toggle_time)
      {
        pre_time = millis();
        ledToggle(led_ch);
      }
    }

    ret = true;
  }


  if (ret != true)
  {
    cliPrintf("led toggle ch[1~%d] time_ms\n", LED_MAX_CH);
  }
}


#endif


#endif
