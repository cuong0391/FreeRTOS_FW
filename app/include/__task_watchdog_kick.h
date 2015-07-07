/**
  ******************************************************************************
  * @file    __task_watchdog_kick.h
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-19-2015
  * @brief   This file contains definitions for #Watchdog task driver
  ******************************************************************************
  */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TASK_WATCHDOG_KICK_H_
#define __TASK_WATCHDOG_KICK_H_

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "hal_wdg.h"

/* Exported defines  -------------------------------------------------------- */
 #define __TASK_WATCHDOG_KICK_TIME_REFRESH               1000
/* Exported functions ------------------------------------------------------- */

/**
 * @brief __task_watchdog_kick
 * The function task will be add to os kernel
 * @param #vParameter poit to parameter if used
 * @return None
 */
void __task_watchdog_kick( void *vParameter );

#endif /* __TASK_WATCHDOG_KICK_H_ */
