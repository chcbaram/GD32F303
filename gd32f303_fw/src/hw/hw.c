/*
 * hw.c
 *
 *  Created on: 2022. 5. 28.
 *      Author: Baram
 */


#include "hw.h"





bool hwInit(void)
{
  bspInit();

  cliInit();
  ledInit();
  uartInit();
  uartOpen(_DEF_UART1, 115200);

  usbInit();
  usbBegin(USB_CDC_MODE);


  return true;
}
