/*
 * hw.h
 *
 *  Created on: 2022. 5. 28.
 *      Author: Baram
 */

#ifndef SRC_HW_HW_H_
#define SRC_HW_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"


#include "led.h"
#include "cdc.h"
#include "usb.h"
#include "uart.h"
#include "cli.h"


bool hwInit(void);



#ifdef __cplusplus
}
#endif

#endif /* SRC_HW_HW_H_ */
