/**
  ******************************************************************************
  * @file    hal_pwm.h
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-03-2015
  * @brief   This file contains definitions for PWM driver
  ******************************************************************************
  */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HAL_PWM_H_
#define _HAL_PWM_H_

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "std_type.h"

/* Exported define ----------------------------------------------------------*/

/** for developer */
#define HAL_PWM_DEBUG               STD_OFF

    
/* Exported typedef ----------------------------------------------------------*/

/**
 * @brief hal_pwm_return_t
 *
 * pwm return type
 */
typedef enum _hal_pwm_return{
    PWM_OK,
    PWM_NULL_PTR,
    PWM_UNKNOWN_ERROR,
}hal_pwm_return_t;

/**
 * @brief hal_pwm_chanel_t
 *
 * Support PWM channels
 */
typedef enum _hal_pwm_channel {
    HAL_PWM_CH_1,
    HAL_PWM_CH_2,
    HAL_PWM_CH_3,
    HAL_PWM_CH_4,
    HAL_PWM_MAX_CHANNEL
}hal_pwm_chanel_t;

/**
 * @brief hal_pwm_t
 *
 * #PWM driver structure
 */
typedef struct _hal_pwm {
    hal_pwm_chanel_t            chid;               /** hal pwm channel id */
    uint32_t                    duty_cycle;         /** hal pwm duty cycle */
    uint32_t                    period;             /** hal pwm duty min */
}hal_pwm_t;


/* Exported functions ------------------------------------------------------- */

/**
 * @brief hal_pwm_init
 * The function shall initialize independence pwm channel
 * @param pwm Point to hal_pwm_t structure
 * @return reference to #hal_pwm_return_t
 */
int hal_pwm_init( void );

/**
 * @brief hal_pwm_start
 * The function starts independence pwm channel
 * @param pwm Point to hal_pwm_t structure
 * @return reference to #hal_pwm_return_t
 */
int hal_pwm_start( hal_pwm_t * pwm );

/**
 * @brief hal_pwm_stop
 * The function stops independence pwm channel
 * @param pwm Point to hal_pwm_t structure
 * @return reference to #hal_pwm_return_t
 */
int hal_pwm_stop( hal_pwm_t * pwm );

/**
 * @brief hal_pwm_set_dutycycle
 * The function sets dutycycle independence pwm channel
 * @param pwm Point to hal_pwm_t structure
 * @return reference to #hal_pwm_return_t
 */
int hal_pwm_set_dutycycle( hal_pwm_t * pwm );

/**
 * @brief hal_pwm_set_period
 * The function update #period for pwm channel
 * #prescaler ( if need ),#dir direction
 * @param pwm Point to hal_pwm_t structure
 * @return reference to #hal_pwm_return_t
 */
int hal_pwm_update_period( hal_pwm_t * pwm );

/**
 * @brief hal_pwm_deinit
 * The function shall de-init by stop clock peripheral hardware PWM  
 * @param pwm Point to hal_pwm_t structure
 * @return reference to #hal_pwm_return_t
 */
int hal_pwm_deinit( void );


#endif /* _HAL_PWM_H_ */