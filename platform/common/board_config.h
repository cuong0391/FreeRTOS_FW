/**
  ******************************************************************************
  * @file    board_config.h
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    06-22-2015
  * @brief   Configuration for board version
  ******************************************************************************
  */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "hal_com.h"
#include "hal_wdg.h"

/* Exported define------------------------------------------------------------*/


/** Config for debug port used in #log.c , reference parameters in #hal_com.h */


#define CFG_LOG_DEBUG_PORT                   COM3
#define CFG_LOG_DEBUG_BAUDRATE               COM_BAUD_115200
#define CFG_LOG_DEBUG_PRIORITY               COM_PRIO_4

#endif /* __BOARD_CONFIG_H__ */
