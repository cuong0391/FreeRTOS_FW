/**
  ******************************************************************************
  * @file    hal_wdg.h
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-11-2015
  * @brief   This file header for #Watchdog driver
  ******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "hal_wdg.h"
#include "stm32f4xx.h"

/* Privated Define------------------------------------------------------------*/

/* Default Key Enable */
#define WDG_CFG_DEFAULT_KEY_EN                0x5555        /* no edit */
/* Default Key Reload */
#define WDG_CFG_DEFAULT_KEY_RELOAD            0xAAAA        /* no edit */
/* Default Key Start */
#define WDG_CFG_DEFAULT_KEY_START             0xCCCC        /* no edit */
/* Maximum value of reload register */
#define WDG_CFG_MAX_VALUE_RELOAD              0xFFF         /* no edit */

/* Exported functions---------------------------------------------------------*/

/*
 * hal_wdg_init
 * The function initialize hardware watchdog 
 */
void hal_wdg_init(void)
{
    /*  enable write to PR, RLR */
    IWDG->KR  = WDG_CFG_DEFAULT_KEY_EN;               
    /* Init prescaler, default prescaler divider is 256 */
    IWDG->PR  = 6;                                       
    /* timeout period in milisecond, minimum is 1000 ms */
    if(WDG_CFG_IWDG_TIMEOUT > 1000)
    {
        /* init value for reload register */
        IWDG->RLR = WDG_CFG_MAX_VALUE_RELOAD & ((WDG_CFG_IWDG_TIMEOUT/1000)*128);
    }
    else
    {
        IWDG->RLR = 128;   
    }
    /* Reload the watchdog */
    IWDG->KR  = WDG_CFG_DEFAULT_KEY_RELOAD;  
}

/*
 * hal_wdg_start
 * The function starts hardware watchdog 
*/
void hal_wdg_start(void)
{
    /* writting value for starts */
    IWDG->KR  = WDG_CFG_DEFAULT_KEY_START;  
}

/*
 * hal_wdg_stop 
 * The function stops hardware watchdog 
 */
void hal_wdg_stop(void)
{
    /* do nothing */
}

/*
 * @brief  hal_wdg_refresh
 * The function refreshes hardware watchdog 
 */
void hal_wdg_refresh(void)
{
    /* the watchdog refreshes */
    IWDG->KR  = WDG_CFG_DEFAULT_KEY_RELOAD;
}
