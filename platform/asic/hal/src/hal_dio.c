/**
  ******************************************************************************
  * @file    hal_dio.c
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-04-2015
  * @brief   This file contains definitions for #dio driver
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "hal_dio.h"
/** #ASIC Hardware target */
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

/* Private typedef -----------------------------------------------------------*/

/* Hatdware dio port struct */
typedef struct _hal_dio_port
{
  uint32_t               Clock;
  GPIO_TypeDef           *Port;
  uint16_t               PinNum;
}hal_dio_port_t;

/* Private macro -------------------------------------------------------------*/
#define CREATE_GPIO(clock, port, pin) \
    { \
        .Clock  = clock, \
        .Port   = port, \
        .PinNum = pin \
    }

/* Private variables ---------------------------------------------------------*/
/* ASIC Hardware dio pin configure parameters */
static const hal_dio_port_t DIO_cfg[] = 
{
    /* HAL_DIO_CH1 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOG,GPIOG,GPIO_Pin_5),
    /* HAL_DIO_CH2 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOD,GPIOD,GPIO_Pin_13),
    /* HAL_DIO_CH3 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOF,GPIOF,GPIO_Pin_13),
    /* HAL_DIO_CH4 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOF,GPIOF,GPIO_Pin_14),   
    /* HAL_DIO_CH5 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOF,GPIOF,GPIO_Pin_12),          
    /* HAL_DIO_CH6   */
    CREATE_GPIO(RCC_AHB1Periph_GPIOE,GPIOE,GPIO_Pin_4),   
    /* HAL_DIO_CH7 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOC,GPIOC,GPIO_Pin_6),
    /* HAL_DIO_CH8 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOC,GPIOC,GPIO_Pin_7),
    /* HAL_DIO_CH9 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOB,GPIOB,GPIO_Pin_0),
    /* HAL_DIO_CH10 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOB,GPIOB,GPIO_Pin_1),   
    /* HAL_DIO_CH11 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOB,GPIOB,GPIO_Pin_6),          
    /* HAL_DIO_CH12 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOB,GPIOB,GPIO_Pin_7),
    /* HAL_DIO_CH13 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOD,GPIOD,GPIO_Pin_14),
    /* HAL_DIO_CH14 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOD,GPIOD,GPIO_Pin_15),
    /* HAL_DIO_CH15 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOH,GPIOH,GPIO_Pin_10),
    /* HAL_DIO_CH16 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOH,GPIOH,GPIO_Pin_12),   
    /* HAL_DIO_CH17 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOH,GPIOH,GPIO_Pin_12),          
    /* HAL_DIO_CH18 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOI,GPIOI,GPIO_Pin_6),
    /* HAL_DIO_CH19 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOI,GPIOI,GPIO_Pin_10),
    /* HAL_DIO_CH20 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOI,GPIOI,GPIO_Pin_11),
    /* HAL_DIO_CH21 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOI,GPIOI,GPIO_Pin_8),
    /* HAL_DIO_CH22 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOH,GPIOH,GPIO_Pin_2),   
    /* HAL_DIO_CH23 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOF,GPIOF,GPIO_Pin_2),          
    /* HAL_DIO_CH24 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOF,GPIOF,GPIO_Pin_3),        
    /* HAL_DIO_CH25 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOF,GPIOF,GPIO_Pin_4),
    /* HAL_DIO_CH26 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOF,GPIOF,GPIO_Pin_5),
    /* HAL_DIO_CH27 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOF,GPIOF,GPIO_Pin_6),
    /* HAL_DIO_CH28 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOF,GPIOF,GPIO_Pin_7),
    /* HAL_DIO_CH29 */
    CREATE_GPIO(RCC_AHB1Periph_GPIOA,GPIOA,GPIO_Pin_4),
};
/* Exported functions --------------------------------------------------------*/
/*
 * hal_dio_init
 * The function shall be initialize independence dio channel
 */
int hal_dio_init( hal_dio_t * dio )
{
    GPIO_InitTypeDef GPIO_InitStruct;
    int ret;
#if (HAL_DIO_DEBUG == STD_ON)         
    /* make sure pwm is invalid structure */
    if(dio != (void*)0)
    {
        /* check if dio port selected is valid */
        if(HAL_DIO_MAX <= dio->chid)
        {
            /* dio is not valid */
            ret = (int)DIO_INVALID_PORT; 
        }
        else
#endif /* HAL_DIO_DEBUG */            
        {
            /* check dir init */
            if(HAL_DIO_OUT == dio->dir)
            {
                /* enable port clock */
                RCC_AHB1PeriphClockCmd(DIO_cfg[dio->chid].Clock,ENABLE); 
                /* config dio */
                GPIO_InitStruct.GPIO_Pin   = DIO_cfg[dio->chid].PinNum;
                GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
                GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
                GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
                GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
                GPIO_Init(DIO_cfg[dio->chid].Port, &GPIO_InitStruct);
                /* set default dio state */
                if(dio->state == HAL_DIO_HIGH)
                {
                    GPIO_SetBits(
                              DIO_cfg[dio->chid].Port, 
                              DIO_cfg[dio->chid].PinNum);
                }
                else
                {
                    GPIO_ResetBits(
                              DIO_cfg[dio->chid].Port, 
                              DIO_cfg[dio->chid].PinNum);
                }
            }
            else
            {
                /* enable port clock */
                RCC_AHB1PeriphClockCmd(DIO_cfg[dio->chid].Clock,ENABLE); 
                /* config dio */
                GPIO_InitStruct.GPIO_Pin   = DIO_cfg[dio->chid].PinNum;
                GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
                GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
                GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
                GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
                GPIO_Init(DIO_cfg[dio->chid].Port, &GPIO_InitStruct);
                /* read default dio level */
                if(GPIO_ReadInputDataBit(
                                    DIO_cfg[dio->chid].Port, 
                                    DIO_cfg[dio->chid].PinNum))
                {
                    dio->state = HAL_DIO_HIGH; }
                else
                {
                    dio->state = HAL_DIO_LOW;
                }
            }
            ret = (int)DIO_OK;     
        }
#if (HAL_DIO_DEBUG == STD_ON) 
    }
    else
    {
        ret = (int)DIO_NULL_PTR;
    }
#endif /* HAL_DIO_DEBUG */    
    return ret;
}

/*
 * hal_dio_set_low
 * The function sets low logic level on #dio channel
 */
int hal_dio_set_low( hal_dio_t * dio )
{
    int ret = DIO_UNKNOWN_ERROR;
#if (HAL_DIO_DEBUG == STD_ON)    
    if((void*)0 == dio)
    {
        ret = (int)DIO_NULL_PTR;
    }
    /* check if dio port selected is valid */
    else if(HAL_DIO_MAX <= dio->chid)
    {
        /* dio is not valid */
        ret = (int)DIO_INVALID_PORT; 
    }
    else
#endif /* HAL_DIO_DEBUG */        
    {
        GPIO_ResetBits(
                      DIO_cfg[dio->chid].Port, 
                      DIO_cfg[dio->chid].PinNum);
        /* update back dio upper driver */
        dio->state = HAL_DIO_LOW;
        ret = (int)DIO_OK;
    }
    return ret;
}

/*
 * hal_dio_set_high
 * The function sets high logic level on #dio channel
 */
int hal_dio_set_high( hal_dio_t * dio )
{
    int ret = DIO_UNKNOWN_ERROR;
#if (HAL_DIO_DEBUG == STD_ON)     
    if((void*)0 == dio)
    {
        ret = (int)DIO_NULL_PTR;
    }
    /* check if dio port selected is valid */
    else if(HAL_DIO_MAX <= dio->chid)
    {
        /* dio is not valid */
        ret = (int)DIO_INVALID_PORT; 
    }
    else
#endif /* HAL_DIO_DEBUG */        
    {
        GPIO_SetBits(
                      DIO_cfg[dio->chid].Port, 
                      DIO_cfg[dio->chid].PinNum);
        /* update back dio upper driver */
        dio->state = HAL_DIO_HIGH;
        ret = (int)DIO_OK;
    }
    return ret;
}

/*
 * hal_dio_toggle
 * The function toggles logic level #dio channel
 */
int hal_dio_toggle( hal_dio_t * dio )
{
    int ret = DIO_UNKNOWN_ERROR;
#if (HAL_DIO_DEBUG == STD_ON)         
    if((void*)0 == dio)
    {
        ret = DIO_NULL_PTR;
    }
    /* check if dio port selected is valid */
    else if(HAL_DIO_MAX <= dio->chid)
    {
        /* dio is not valid */
        ret = (int)DIO_INVALID_PORT; 
    }
    /* check valid output direction */
    else if(HAL_DIO_OUT != dio->dir)
    {
        /* dio is not output */
        ret = (int)DIO_UNKNOWN_ERROR;
    }
    else
#endif /* HAL_DIO_DEBUG */        
    {
        if(HAL_DIO_LOW == dio->state)
        {
            GPIO_SetBits(
                      DIO_cfg[dio->chid].Port, 
                      DIO_cfg[dio->chid].PinNum);
            /* update back dio upper driver */
            dio->state = HAL_DIO_HIGH;
        }
        else
        {
            GPIO_ResetBits(
                      DIO_cfg[dio->chid].Port, 
                      DIO_cfg[dio->chid].PinNum);
            /* update back dio upper driver */
            dio->state = HAL_DIO_LOW;
        }
        ret = (int)DIO_OK;
    }
    return ret;
}

/*
 * hal_dio_read
 * The function reads logic level on #dio channel
 */
hal_dio_level_t hal_dio_read( hal_dio_t * dio )
{
    hal_dio_level_t ret = HAL_DIO_LOW;
#if (HAL_DIO_DEBUG == STD_ON)         
    if((void*)0 == dio)
    {
        ret = dio->state;
    }
    /* check if dio port selected is valid */
    else if(HAL_DIO_MAX <= dio->chid)
    {
        /* dio is not valid */
        ret = dio->state; 
    }
    /* check valid input direction */
    else if(HAL_DIO_IN != dio->dir)
    {
        /* dio is not input */
        ret = dio->state;
    }
    else
#endif /* HAL_DIO_DEBUG */        
    {
        ret = GPIO_ReadInputDataBit(
                                DIO_cfg[dio->chid].Port, 
                                DIO_cfg[dio->chid].PinNum) 
                                ? HAL_DIO_HIGH : HAL_DIO_LOW;
        dio->state = ret;
    }
    return ret;
}
