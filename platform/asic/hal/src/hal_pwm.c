/**
  ******************************************************************************
  * @file    hal_pwm.c
  * @author    Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-03-2015
  * @brief   This file expandes for #PWM driver
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "hal_pwm.h"
/** #ASIC Hardware target */
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

/* Exported functions --------------------------------------------------------*/
/*
 * hal_pwm_init
 * The function shall initialize independence pwm channel
 */
int hal_pwm_init( void )
{
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
    TIM_OCInitTypeDef          TIM_OCInitStructure;
    GPIO_InitTypeDef           GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1); 
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1); 

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler     = (uint16_t) ((SystemCoreClock /2) / 21000000) - 1;
    /*
     * PWM 20KHz  = 1000
     * PWM 10KHz  = 2000
     * PWM 7.5KHz = 3000
     * PWM 5KHz   = 4000
     */      
    TIM_TimeBaseStructure.TIM_Period        = 1000;   
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    //TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);  
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM1, ENABLE);

    /* TIM1 enable counter */
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    return (int)PWM_OK;
}

/*
 * hal_pwm_start
 * The function starts independence pwm channel
 */
int hal_pwm_start( hal_pwm_t * pwm )
{
    int ret;
#if (HAL_PWM_DEBUG == STD_ON)    
    /* make sure pwm is invalid structure */
    if(pwm != (void*)0)
#endif /* HAL_PWM_DEBUG */        
    {        
        switch(pwm->chid)
        {
            case HAL_PWM_CH_1:
                /* update new duty cycle for pwm ch1 */
                TIM_SetCompare1(TIM1,pwm->duty_cycle);
                break;
            case HAL_PWM_CH_2:
                /* update new duty cycle for pwm ch2 */
                TIM_SetCompare2(TIM1,pwm->duty_cycle);
                break;
            case HAL_PWM_CH_3:
                /* update new duty cycle for pwm ch3 */
                TIM_SetCompare3(TIM1,pwm->duty_cycle);
                break;
            case HAL_PWM_CH_4:
                /* update new duty cycle for pwm ch4 */
                TIM_SetCompare4(TIM1,pwm->duty_cycle);      
                break;
            default : 
                break;
        }
        ret = (int)PWM_OK; 
    }
#if (HAL_PWM_DEBUG == STD_ON)      
    else
    {
        ret = (int)PWM_NULL_PTR;
    }
#endif /* HAL_PWM_DEBUG */    
    return ret;
}

/*
 * hal_pwm_stop
 * The function stops independence pwm channel
 */
int hal_pwm_stop( hal_pwm_t * pwm )
{
    int ret;
#if (HAL_PWM_DEBUG == STD_ON)      
    /* make sure pwm is valid structure */
    if(pwm != (void*)0)
#endif /* HAL_PWM_DEBUG */        
    {
        switch(pwm->chid)
        {
            case HAL_PWM_CH_1:
                /* update new duty cycle for pwm ch1 */
                TIM_SetCompare1(TIM1,0);
                break;
            case HAL_PWM_CH_2:
                /* update new duty cycle for pwm ch2 */
                TIM_SetCompare2(TIM1,0);
                break;
            case HAL_PWM_CH_3:
                /* update new duty cycle for pwm ch3 */
                TIM_SetCompare3(TIM1,0);
                break;
            case HAL_PWM_CH_4:
                /* update new duty cycle for pwm ch4 */
                TIM_SetCompare4(TIM1,0);      
                break;
            default : 
                break;
        }
        ret = (int)PWM_OK; 
    }
#if (HAL_PWM_DEBUG == STD_ON)    
    else
    {
        ret = (int)PWM_NULL_PTR;
    }
#endif /* HAL_PWM_DEBUG */    
    return ret;
}

/*
 * hal_pwm_set_dutycycle
 * The function sets dutycycle independence pwm channel
 */
int hal_pwm_set_dutycycle( hal_pwm_t * pwm )
{
    int ret = PWM_UNKNOWN_ERROR;
#if (HAL_PWM_DEBUG == STD_ON)        
    /* make sure pwm is valid structure */
    if(pwm != (void*)0)
#endif /* HAL_PWM_DEBUG */        
    {
        switch(pwm->chid)
        {
            case HAL_PWM_CH_1:
                /* update new duty cycle for pwm ch1 */
                TIM_SetCompare1(TIM1,pwm->duty_cycle);
                break;
            case HAL_PWM_CH_2:
                /* update new duty cycle for pwm ch2 */
                TIM_SetCompare2(TIM1,pwm->duty_cycle);
                break;
            case HAL_PWM_CH_3:
                /* update new duty cycle for pwm ch3 */
                TIM_SetCompare3(TIM1,pwm->duty_cycle);
                break;
            case HAL_PWM_CH_4:
                /* update new duty cycle for pwm ch4 */
                TIM_SetCompare4(TIM1,pwm->duty_cycle);
                break;
            default: break;           
        }
        /* set duty cycle success */
        ret = (int)PWM_OK;
    }
#if (HAL_PWM_DEBUG == STD_ON)    
    else
    {
        ret = (int)PWM_NULL_PTR;
    }
#endif /* HAL_PWM_DEBUG */    
    return ret;
}

/**
 * hal_pwm_update_period
 * The function update #period for pwm channel
 */
int hal_pwm_update_period( hal_pwm_t * pwm )
{
    int ret = PWM_UNKNOWN_ERROR;
#if (HAL_PWM_DEBUG == STD_ON)    
    /* make sure pwm is valid structure */
    if(pwm != (void*)0)
#endif /* HAL_PWM_DEBUG */        
    {
        /* Generate an update event to reload the Prescaler 
        and the repetition counter(only for TIM1 and TIM8) value immediatly */
        switch(pwm->chid)
        {
            case HAL_PWM_CH_1:
                /* update new #period for #pwm ch1 */
                TIM1->ARR = pwm->period;
                break;
            case HAL_PWM_CH_2:
                /* update new #period for #pwm ch2 */
                TIM1->ARR = pwm->period;
                break;
            case HAL_PWM_CH_3:
                /* update new #period for #pwm ch3 */
                TIM1->ARR = pwm->period;
                break;
            case HAL_PWM_CH_4:
                /** update new #period for #pwm ch4 */
                TIM1->ARR = pwm->period;
                break;
            default: break;           
        }     
        TIM1->EGR = TIM_PSCReloadMode_Immediate; 
        ret = (int)PWM_OK;
    }
#if (HAL_PWM_DEBUG == STD_ON)    
    else
    {
        ret = (int)PWM_NULL_PTR;
    }
#endif /* HAL_PWM_DEBUG */    
    return (int)ret;
}

/**
 * hal_pwm_deinit
 * The function shall de-init by stop clock peripheral hardware PWM  
 */
int hal_pwm_deinit( void )
{
    TIM_DeInit(TIM1);
    return PWM_OK;
}
