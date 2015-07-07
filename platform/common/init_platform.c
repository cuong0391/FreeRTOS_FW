/**
  ******************************************************************************
  * @file    init_platform.c
  * @author  Embedded Team CopyRight@NTC
  * @version V1.1.0
  * @date    June-19-2015
  * @brief   This file contains init_platform function
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
/** Standard library. */
#include "os.h"
#include "misc.h"

/* Exported functions --------------------------------------------------------*/

/**
 * @brief init_platform
 * The function starts all driver
 * @param None
 * @return None
 */
void init_platform( void )
{
    /* debuging lib init */
    log_init();
    /* init watchdog  */
    hal_wdg_init();
    /* init nvic group 4*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}
