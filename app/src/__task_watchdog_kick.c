/**
  ******************************************************************************
  * @file    __task_watchdog_kick.c
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-19-2015
  * @brief   This file contains expand for #Watchdog task driver
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "os.h"
/* Exported functions ------------------------------------------------------- */

/*
 * __task_watchdog_kick
 * 
 */
void __task_watchdog_kick( void *vParameter )
{
    __task_handle_t *hdl = (__task_handle_t *)(vParameter);
    /* start watchdog hardware */
    hal_wdg_start();
    /* task ready id */
    hdl->state = __TASK_READY;
    /* task start */
    log_msg("Start %s\r\n",__FUNCTION__);
    /* loop forever */
    for(;;)
    {
        /* refreshes watchdog timer, if don't refreshes him when timeout 
        reaches by WDG_CFG_IWDG_TIMEOUT, he will reset your system when the counter
        register over value is configured.
        */
        hal_wdg_refresh();
        /* task sleep __TASK_WATCHDOG_KICK_TIME_REFRESH */
        vTaskDelay(__TASK_WATCHDOG_KICK_TIME_REFRESH);
    }
}
