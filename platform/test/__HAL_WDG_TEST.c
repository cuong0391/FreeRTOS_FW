/**
  ******************************************************************************
  * @file    __HAL_GPIO_WDG.c
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-12-2015
  * @brief   This file expandes for Test HAL_WDG driver
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "hal_wdg.h"
#include "hal_gpio.h"
/* Private variables ---------------------------------------------------------*/
static hal_gpio_t  gpio ={
        .chid  = HAL_GPIO_1,
        .dir   = HAL_GPIO_OUT,
        .state = HAL_GPIO_HIGH
};
/* Exported functions ------------------------------------------------------- */
static void delay (uint32_t time);
int main (void)
{
    hal_wdg_init();
    hal_gpio_init(&gpio);
    hal_wdg_start();
    delay(1000);
    while(1)
    {
        delay(1000);
        hal_gpio_set_low(&gpio);
        /* doesn't refresh wathdog */
        //while(1);
        hal_wdg_refresh();
    }
    return 0;
}
static void delay (uint32_t time)
{
    for(time=time *10000; time>0;time-- ){}
}
