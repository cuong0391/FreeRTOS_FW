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
#include "std_type.h"
#include "hal_com.h"
#include "hal_wdg.h"

/* Exported define------------------------------------------------------------*/
/** for development software debuging */
#define LOG_VERBOSE                          4
#define LOG_VERBOSE_DEFAULT                  3

#define LOG_MSG                              STD_ON
#define LOG_DEBUG                            STD_ON
#define LOG_INFO                             STD_ON
#define LOG_WARN                             STD_ON
#define LOG_ERROR                            STD_ON



/** Config for debug port used in #log.c , reference parameters in #hal_com.h */


#define CFG_LOG_DEBUG_PORT                   COM3
#define CFG_LOG_DEBUG_BAUDRATE               COM_BAUD_115200
#define CFG_LOG_DEBUG_PRIORITY               COM_PRIO_4

#endif /* __BOARD_CONFIG_H__ */
