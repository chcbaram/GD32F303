/*
 * hw_def.h
 *
 *  Created on: 2022. 5. 28.
 *      Author: Baram
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "def.h"
#include "bsp.h"


#define _DEF_FIRMWATRE_VERSION    "V220528R1"
#define _DEF_BOARD_NAME           "GD32F303"



#define _USE_HW_LED
#define      HW_LED_MAX_CH          1

#define _USE_HW_USB
#define _USE_HW_CDC
#define      HW_USE_CDC             1
#define      HW_USE_MSC             0

#define _USE_HW_UART
#define      HW_UART_MAX_CH         1

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    16
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    64



#endif /* SRC_HW_HW_DEF_H_ */
