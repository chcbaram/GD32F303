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

  ledInit();

  return true;
}
