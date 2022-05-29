/*
 * bsp.h
 *
 *  Created on: 2022. 5. 28.
 *      Author: Baram
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"


#include "gd32f30x_libopt.h"



bool bspInit(void);
bool bspDeInit(void);

void delay(uint32_t time_ms);
uint32_t millis(void);


//void logPrintf(const char *fmt, ...);
#define logPrintf printf


#ifdef __cplusplus
}
#endif

#endif /* SRC_BSP_BSP_H_ */
