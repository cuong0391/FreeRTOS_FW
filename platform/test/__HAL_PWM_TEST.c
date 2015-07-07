/**
  ******************************************************************************
  * @file    __HAL_GPIO_PWM.c
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-12-2015
  * @brief   This file expandes for Test HAL_PWM driver
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "hal_pwm.h"

/* Private variables ---------------------------------------------------------*/
hal_pwm_t pwm1 ={
        .chid = HAL_PWM_CH_1,
        .dir  = HAL_PWM_POLARITY_HIGH,
        .duty_cycle = 500
};

hal_pwm_t pwm2 ={
        .chid = HAL_PWM_CH_2,
        .dir  = HAL_PWM_POLARITY_HIGH,
        .duty_cycle = 500
};

hal_pwm_t pwm3 ={
        .chid = HAL_PWM_CH_3,
        .dir  = HAL_PWM_POLARITY_HIGH,
        .duty_cycle = 500
};

hal_pwm_t pwm4 ={
        .chid = HAL_PWM_CH_4,
        .dir  = HAL_PWM_POLARITY_HIGH,
        .duty_cycle = 500
};
/* Exported functions ------------------------------------------------------- */

void __delay_ms(uint32_t time)
{
    uint32_t i = 0,j = 0;

    for(;j < time; j++)
    {
        for(i = 0; i < time;i++)
        {
            ;
        }
    }

}

int main (void)
{
    uint32_t duty = 0;
    hal_pwm_init();
    hal_pwm_start(&pwm1);
    hal_pwm_start(&pwm2);
    hal_pwm_start(&pwm3);
    hal_pwm_start(&pwm4);
    while(1)
    {
        if(duty < 1000)
        {
            duty++;
            pwm1.duty_cycle = duty;
            pwm2.duty_cycle = duty;
            pwm3.duty_cycle = duty;
            pwm4.duty_cycle = duty;
            hal_pwm_set_dutycycle(&pwm1);
            hal_pwm_set_dutycycle(&pwm2);
            hal_pwm_set_dutycycle(&pwm3);
            hal_pwm_set_dutycycle(&pwm4);
        }
        else
        {
            duty = 0;
        }
        __delay_ms(400);
    }
    return 0;
}
