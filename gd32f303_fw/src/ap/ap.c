/*
 * ap.c
 *
 *  Created on: 2022. 5. 28.
 *      Author: Baram
 */


#include "ap.h"





void apInit(void)
{

}

void apMain(void)
{
  uint32_t pre_time;


  while(1)
  {
    if (millis()-pre_time >= 500)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);
    }
  }
}
