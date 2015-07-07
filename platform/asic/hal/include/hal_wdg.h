/**
  ******************************************************************************
  * @file    hal_wdg.h
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-11-2015
  * @brief   This file expandes for #Watchdog driver
  ******************************************************************************
*/ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HAL_WDG_H_
#define _HAL_WDG_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported Define------------------------------------------------------------*/

/* Timeout of IWDG (ms) */
#define WDG_CFG_IWDG_TIMEOUT                  5000  

/* Exported functions---------------------------------------------------------*/

/**
 * @brief  hal_wdg_init
 * The function initialize hardware watchdog 
 * @param  None
 * @retval None
 */
void hal_wdg_init(void);

/**
 * @brief  hal_wdg_start
 * The function starts hardware watchdog 
 * @param  None
 * @retval None
 */
void hal_wdg_start(void);

/**
 * @brief  hal_wdg_stop
 * The function stops hardware watchdog 
 * @param  None
 * @retval None
 */
void hal_wdg_stop(void);

/**
 * @brief  hal_wdg_refresh
 * The function refreshes hardware watchdog 
 * @param  None
 * @retval None
 */
void hal_wdg_refresh(void);

#endif /* _HAL_WDG_H_ */